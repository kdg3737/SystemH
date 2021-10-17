# What
Maybe you're a C++ developer looking to improve productivity, maybe a .Net developer eyeing C++ for portability or a performance boost, or maybe you just found a piece of C# code you want to efficiently translate to C++. This project might be exactly what you're looking for. 

SystemH is an attempt to bring the .Net framework to C++, only without the .Net using just C++11 native code. All (ok, 'most') code is well tested, both functionally and for performance. Compared to equivalent .Net code you can expect on average a 10-20% increase in performance, athough in some cases like with System::Collections::Generic::Dictionary it can be over twice as fast.
# Usage
Simpy include the System.h file in your project and start using all your favorite .Net classes like String, Func, Tuple, List, Dictionary, DateTime, Thread and many more. Note that on ESP32 or Arduino you'll need to explicitly activate the integrated memorypool by adding 'UseMemoryPool = true;' in the setup function of your sketch.
# Documentation
There is none and I don't plan to write any anytime soon. Check out the tests and the code itself to figure out how everything works.
# License
This code is licensed under the GNU AGPL, the strongest open-source license I could find, cause I'm just paranoid like that. If you need a less restrictive license contact me.
# Contact
krisdegreve@hotmail.com