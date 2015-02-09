#include <v8.h>  
using namespace v8;  
int main(int argc, char* argv[]) {    
	// Create a stack-allocated handle scope.   
	HandleScope handle_scope;    
	// Create a new context.   
	Persistent<Context> context = Context::New();      
	// Enter the created context for compiling and   
	// running the hello world script.    
	Context::Scope context_scope(context);    
	// Create a string containing the JavaScript source code.   
	//Handle<String> source = String::New("'Hello' + ', World!'");    
	Handle<String> source = String::New("a = {'3',6,8}");    
	// Compile the source code.   
	Handle<Script> script = Script::Compile(source);      
	// Run the script to get the result.   
	Handle<Value> result = script->Run();      
	// Dispose the persistent context.   
	context.Dispose();    
	// Convert the result to an ASCII string and print it.   
	String::AsciiValue ascii(result);   
	printf("%s\n", *ascii);   return 0; 
}
//g++ -g main.cpp libv8.a -o test.exe -I ~/v8-read-only/include -lpthread
