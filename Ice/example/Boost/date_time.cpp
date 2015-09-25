#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/date_time/local_time/local_time.hpp> 
#include <iostream>	
using namespace std;
int	main()	
{	  
	using namespace boost::gregorian;	  
	using namespace boost::posix_time;
	using namespace boost::local_time;
	ptime p(not_a_date_time);
	std::stringstream ss;
	 local_time_input_facet* input_facet = new local_time_input_facet();
	ss.imbue(locale(ss.getloc(), input_facet));
	//input_facet->format("%m %d %Y");
	//input_facet->format("%Y-%m-%d %H:%M:%S");
	input_facet->format("%Y-%m-%d %H:%M:%S");
	//ss.str("03 12 2008");
	//ss.str("2012/05/02 00:00:00");
	ss.str("2011-01-08 00:00:00");
	ss >> p;
	//p = time_from_string("2012/5/3 00:00:00");
	cout << p << endl;
	cout << "====================" << endl;
	cout << endl;
	//date d(2000,Jan,20);
	/*date d = from_string("2000/01/01");
	ptime start(d);
	ptime end = start + hours(1);
	time_iterator titr(start,minutes(15)); 
	while (titr < end) {
		std::cout << to_simple_string(*titr) << std::endl;	    
		++titr;	  
	}	  
	std::cout << "Now backward" << std::endl;*/
	//ptime p1 = time_from_string("2010/02/22 14:19:43");
	/*ptime p1 = from_time_t(1323886019);
	time_zone_ptr zone( new posix_time_zone("GMT+8"));
	local_date_time  t (p1, zone);
	cout << t << endl;*/

	//ptime p1 = from_time_t(1323886019);
	//std::cout << "date : " << p1.date() << std::endl;
	//std::cout << "week : " << p1.date().day_of_week() << std::endl;
	//std::cout << "hours : " << p1.time_of_day().hours() << std::endl;
	//time_zone_ptr zone( new posix_time_zone("GMT+8"));
	//local_date_time  t (p1, zone);
	//cout << t.local_time().time_of_day().hours()<< endl;
	//cout << t << endl;
	//std::cout << "date : " << p1.date() << std::endl;
	//std::cout << "week : " << p1.date().day_of_week() << std::endl;
	//std::cout << "hours : " << p1.time_of_day().hours() << std::endl;
	/*time_t tt = 1323886019;
	tm* tm2 = localtime(&tt);
	ptime p = ptime_from_tm(*tm2);
	std::cout << p.date().month() << std::endl;
	date_facet *tfacet = new date_facet("%m-%d-%Y");
	ostringstream os;
	os.imbue(std::locale(std::cout.getloc(), tfacet));
	os << p.time_of_day().hours();//.month();
	std::cout << os.str() << std::endl;*/
	ptime p2 = second_clock::local_time();
	int week = p2.date().day_of_week();
	std::cout << week << std::endl;
	date_facet *tfacet = new date_facet("%Y-%m-%d");
	std::cout.imbue(std::locale(std::cout.getloc(), tfacet));
	std::cout << p2.date() << std::endl;
	std::cout << p2.time_of_day().hours() << std::endl;
	std::cout << p2.time_of_day().minutes() << std::endl;
	std::cout << p2.time_of_day().seconds() << std::endl;
	//ptime p1 = time_from_string("02-22-2010 14:19:43");
	/*std::cout << "P1:" << to_simple_string(p1) << std::endl;
	ptime p2 = second_clock::local_time();
	p1 = p2;
	std::cout << "P2:" << to_simple_string(p2) << std::endl;
	time_duration p3 = p2-p1;
	std::cout << "p2-p1:" << p3.hours() << std::endl;

	ptime p4 = p1 - hours(3);
	std::cout << "p1-3h:" << to_simple_string(p4) << std::endl;
	std::cout <<  p1.date().year() << "/" <<
		   p1.date().month() << "/"  <<  p1.date().day() <<  " " << 
		   int(p1.date().day_of_week()) << ":"  << 
		   p1.time_of_day().hours() <<  ":"  << 
		   p1.time_of_day().minutes() <<  ":" <<
		   p1.time_of_day().seconds() << std::endl;
	//time_facet tfacet("%Y/%m/%d %H:%M:%S");
	time_facet *tfacet = new time_facet("%m-%d-%Y %H:%M:%S");
	std::cout.imbue(std::locale(std::cout.getloc(), tfacet));
	std::cout << p1 << std::endl;

	{
		ostringstream os;
		os.imbue(std::locale(std::cout.getloc(), tfacet));
		os << p1;

		cout << os.str() << endl;
	}*/
}



//g++ -g -Wall datetest.cpp -o datetest.exe -lboost_date_time
