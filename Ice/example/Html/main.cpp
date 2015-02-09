#include <htmlcxx/html/ParserDom.h>  
#include <iostream>

using namespace std;
using namespace htmlcxx;
using namespace HTML;

int main()
{
	//Parse some html code  
	string html = "<html><body><A href=\"www.baidu.com\"/>hey<br></body></html>";  
	cout << html << endl;
	HTML::ParserDom parser;  
	tree<HTML::Node> dom;
	dom = parser.parseTree(html);    
	//Print whole DOM tree  
	cout << dom << endl;    
	//Dump all links in the tree  
	tree<HTML::Node>::iterator it = dom.begin();  
	tree<HTML::Node>::iterator end = dom.end();  
	for (; it != end; ++it)  
	{  	
		cout << it->tagName() << "," << it->closingText() << endl;
		if (it->tagName() == "A")  	
		{  		
			it->parseAttributes();  		
			cout << (it->attribute("href")).second;
		}  
	}    
	cout << endl;
	//Dump all text of the document  
	it = dom.begin();  
	end = dom.end();  
	for (; it != end; ++it)  
	{  	
		if ((!it->isTag()) && (!it->isComment()))  	
		{  		
			cout << it->text();  	
		}  
	}
	cout << endl;
}
