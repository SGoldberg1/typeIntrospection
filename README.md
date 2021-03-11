# typeIntrospection

A simple way to inspect types during runtime.

Source Description: <br/>
MetaGen.cpp: A simple parser that takes in a c++ file to parse and generate meta data for types <br/>
MetaGen.h: Data for the parsing and meta data generation process <br/>
Meta_Generated.h: The output file from MetaGen.cpp.  <br/>
Main.cpp: Usage example <br/>
Main.h: More of the usage example <br/>

We tag a struct with Introspection() so that the MetaGen will know what to generate data for. <br/>
The workflow is to have Metagen run before the program that we want to inspect compiles.  <br/>
Include Meta_Generated.h so that you can have access to the meta data. <br/>

Notes:  <br/>
This does not handle everything, and currently, parameters for 'Introspection()' do nothing. <br/>
If 64 bit build: Warnings about casting a 64 bit address to a 32 bit value may happen...may even be an error. <br/>
Change the offset in meta_struct(in Meta_Generated.h) to a 64 bit value. <br/>
...or turn off the warnings in your compiler...I don't think we are going to have a struct  <br/>
with member variable offsets greater than 2^32 bytes!!! <br/>
Do note the build.sh file pipes the output from the metaGen to a file. MetaGen.cpp uses standard print routines. <br/>
Cheap file generation... <br/>
This program is an example. So, the file read in is hardcoded. <br/>
