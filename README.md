Grapheme-String 

Unicode compatible string libraries always include the disclaimer that whatever units you're working with don't correspond to what look like individual characters (graphemes) on the screen. This simplifies everything by getting rid of that. This is a string library where the unit is the grapheme.

aString[n] returns the nth grapheme, which is also a string.

iterators iterate over graphemes.

You can use for each:
for(auto aGrapheme : aString) something << aGrapheme; 

There are reverse iterators, you could use:
for (auto a = aString.rbegin(); a != aString.rend(); ++a) {
    something << a;/\*etc\*/
}

You can get substrings with slice.  Note that negative indexes index from the end. 

Comparison operators, hashing are predefined, allowing use with standard library objects like map and unordered_map etc.

Note, the way this works is that strings are broken up into graphemes on creation and indexed.  Due to pre-indexing, indexing aftwards is fast, but also strings are immutable. 

What are exposed as strings in this library are actually substrings, where whenever you take a substring, the original string is maintained and the new substring just refers to it.  If you want to release a master string, then replace your substrings with deep copies (there's a deep_copy() function) and when there are no references left to the master string it will be automatically deleted.

This is a library for immutable strings. There is a GraphemeStringBuilder class for building new strings out of bits of existing ones.

So to summarize what these are like:  

1) Creating strings is slow, because they're indexed on creation.  They take up a lot of memory relative to utf8, because they have a UTF8 buffer and a UTF32 buffer and a grapheme index.

2) Making substrings is instant, since substrings are shared.

3) Sharing is done with reference counting which is fast because it's not thread safe (unless you change a line).
a) Strings and their substrings should stay in the thread they were created in or else be transfered all at once. 

4) Strings are immutable but there is a string builder class.
5) Strings are compatible with the standard library, with for each etc. There are iterators, comparisons and hashes.

This version is for Windows and can convert windows 16 bit unicode. 
Later I'll add Linux compatibility.

Output using streams assumes that you want UTF8 output. I realize that this isn't window's preference for display, but I assume that you're keeping your documents in utf8 form and displaying them another way.  Note that every string DOES HAVE an array of 32 bit Codepoints as well as a UTF8 buffer so you could output using those. I didn't make output to streams use windows wchar because those are rather limited and the streams stop working if you output a character that isn't in the default code page - and if you need a grapheme library you're probably using characters that aren't in everyone's default codepage. 

It also uses the Boost Library for reference counted sharing through the intrusive counter.  By default faster, not-thread safe reference counting is used, but you can change it to safe multi-thread reference counting with a one line change.

Boost is the only dependency that isn't already included in the source files.

It has Julia's utf8proc library as a dependency. The latest version of that is included in a subdirectory.

It has Bob Jenkins' spooky hash as a dependency, also included.



