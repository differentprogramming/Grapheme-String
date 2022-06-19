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

What are exposed as strings in this library are actually substrings, where whenever you take a substring, the original string is maintained and the new substring just refers to it.  If you want to release a master string, then replace your substrings with deep copies (there's a deep_copy() function) and when there are no references left to the master string it will be automatically deleted.

Note this is a library for immutable strings. There is a GraphemeStringBuilder class for building new strings out of bits of existing ones.
This version is for Windows and can convert windows 16 bit unicode. 
Later I'll add Linux compatibility.

Output using streams assumes that you want UTF8 output. I realize that this isn't window's preference for display, but I assume that you're keeping your documents in utf8 form and displaying them another way.  Note that every string DOES HAVE an array of 32 bit Codepoints as well as a UTF8 buffer so you could output using those.

It also uses the Boost Library for reference counted sharing through the intrusive counter.  By default faster, not-thread safe reference counting is used, but you can change it to safe multi-thread reference counting with a one line change.

Boost is the only dependency that isn't already included in the source files.

It has Julia's utf8proc library as a dependency. The latest version of that is included in a subdirectory.

It has Bob Jenkins' spooky hash as a dependency, also included.



