#include "grapheme.h"



/*UTF8PROC_NULLTERM UTF8PROC_STABLE UTF8PROC_COMPOSE
UTF8PROC_DLLEXPORT utf8proc_ssize_t utf8proc_map(
  const utf8proc_uint8_t *str, utf8proc_ssize_t strlen, utf8proc_uint8_t **dstptr, utf8proc_option_t options
);

UTF8PROC_DLLEXPORT utf8proc_ssize_t utf8proc_iterate(const utf8proc_uint8_t *str, utf8proc_ssize_t strlen, utf8proc_int32_t *codepoint_ref);


*/
LPSTR UnicodeToUTF8(LPCTSTR s)
{
	int bsize = WideCharToMultiByte(
		CP_UTF8,
		0,
		s,
		-1,
		NULL,
		0,
		NULL,
		NULL
	);
	LPSTR mbuffer = new CHAR[bsize + 1];

	WideCharToMultiByte(
		CP_UTF8,
		0,
		s,
		-1,
		mbuffer,
		bsize + 1,
		NULL,
		NULL
	);
	return mbuffer;
}

uint8_t GraphemeString::nullbyte = 0;
int32_t GraphemeString::nullcodepoint = 0;


void GraphemeString_letter::build(const std::vector<GraphemeString>& o)
{


	int utf8_size = 0;

	for (auto g : o) {
		utf8_size += g.byte_length();
	}

	uint8_t* store_normalized = (uint8_t*)malloc(utf8_size + 1);
	std::unique_ptr<uint8_t, FreeDeleter> buf(store_normalized);

	int offset = 0;
	for (auto g : o) {
		g.fill_utf8(store_normalized + offset);
		offset += g.byte_length();
	}
	load(store_normalized);
}
void GraphemeString_letter::load(const GraphemeString& o1, const GraphemeString& o2)
{
	uint8_t* store_normalized = (uint8_t*)malloc(o1.byte_length() + o2.byte_length() + 1);
	std::unique_ptr<uint8_t, FreeDeleter> holder(store_normalized);
	o1.fill_utf8(store_normalized, false);
	o2.fill_utf8(store_normalized + o1.byte_length());
	load(store_normalized);
}


void GraphemeString_letter::load(const GraphemeString& o)
{
	uint8_t* store_normalized = (uint8_t*)malloc(o.byte_length() + 1);
	std::unique_ptr<uint8_t, FreeDeleter> holder(store_normalized);
	o.fill_utf8(store_normalized);
	load(store_normalized);
}

void GraphemeString_letter::load(const uint8_t* source)
{
	uint8_t* store_normalized;

	int utf8_size = utf8proc_map(source, 0, &store_normalized, (utf8proc_option_t)(UTF8PROC_NULLTERM | UTF8PROC_STABLE | UTF8PROC_COMPOSE) //| UTF8PROC_NLF2LF
	);
	if (utf8_size < 1) {
		switch (utf8_size) {
		case UTF8PROC_ERROR_NOMEM:
			throw std::runtime_error("out of memory in GraphemeString load");
		case UTF8PROC_ERROR_OVERFLOW:
			throw std::runtime_error("string too long to process in GraphemeString load");
		case UTF8PROC_ERROR_INVALIDUTF8:
			throw std::domain_error("invalid unicode in GraphemeString load");
		case UTF8PROC_ERROR_NOTASSIGNED:
			throw std::domain_error("unassigned codepoint in GraphemeString load");
		case UTF8PROC_ERROR_INVALIDOPTS:
			throw std::logic_error("invalide options in GraphemeString load");

		default:
			throw std::runtime_error("unknown runtime error in GraphemeString load");
		}
	}
	utf8_buffer_size = utf8_size;
	utf8_buffer = std::move(std::unique_ptr<uint8_t, FreeDeleter>(store_normalized));
	codepoint_buffer.resize(1, 0);
	codepoint_to_utf8_index.resize(1, 0);
	grapheme_to_codepoint_index.resize(1, 0);
	int grapheme_state = UTF8PROC_BOUNDCLASS_START;

	int32_t prev_codepoint;

	for (int codepoint_index = 0;; ++codepoint_index) {

		codepoint_to_utf8_index.push_back(codepoint_to_utf8_index.back()
			+ utf8proc_iterate(store_normalized + codepoint_to_utf8_index.back(),
				-1,
				&codepoint_buffer.back()));
		if (codepoint_buffer.back() == -1) {
			codepoint_buffer.pop_back();
			codepoint_to_utf8_index.pop_back();
			grapheme_to_codepoint_index.push_back(codepoint_index);
			break;
		}
		if (codepoint_index > 0 && utf8proc_grapheme_break_stateful(prev_codepoint, codepoint_buffer.back(), &grapheme_state)) {
			grapheme_to_codepoint_index.push_back(codepoint_index);
		}
		prev_codepoint = codepoint_buffer.back();
		codepoint_buffer.push_back(0);
	}
}