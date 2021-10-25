# What
SystemH is an attempt to bring the .Net framework to C++, only without the .Net using just C++11 native code. All (ok, 'most') code is well tested, both functionally and for performance. Compared to equivalent .Net code you can expect on average a 10-20% increase in performance, athough in some cases like with System::Collections::Generic::Dictionary it can be over twice as fast.
# Usage
Simply include the System.h file in your project and start using all your favorite .Net classes like String, Func, Tuple, List, Dictionary, DateTime, Thread and many more. 
Here's a simple example to get you started, the C# code:

```C#
	List<string> lst = new List<string>() { "banana", "apple", "orange", "lemon" };
	List<string> filtered = (from str in lst where str.Contains("a") orderby str select str.Substring(0, 3)).ToList();
	for (int i = 0; i < filtered.Count; i++) {
		Console.WriteLine(filtered[i]);
	}
```

Which in SystemH becomes:

```SystemH
	List<string> lst = { "banana", "apple", "orange", "lemon" };
	List<string> filtered = lst.Where([](string str) { return str.Contains("a"); }).OrderBy<string>([](string str) { return str; }).Select<string>([](string str) {return str.Substring(0, 3); }).ToList();
	for (int i = 0; i < filtered.Count; i++) {
		Console::WriteLine(filtered[i]);
	}
```

Note: on ESP32/Arduino you'll need to explicitly activate the integrated memorypool by adding 'UseMemoryPool = true;' in the setup function of your sketch.


Have a nice play!


# Documentation
There is none and I don't plan to write any anytime soon. Check out the tests and the code itself to figure out how everything works.
# License
This code is licensed under the GNU AGPL. If you need a less restrictive or commercial license contact me.
# Contact
krisdegreve@hotmail.com