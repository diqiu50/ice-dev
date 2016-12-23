//
// async_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/asio/high_resolution_timer.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

using boost::asio::ip::udp;
using boost::thread;
using boost::asio::ip::address_v4;
using namespace std;

enum MsgType {
	eVoteReq,
	eVote,
	eHeartbeat,
	eHeartbeatResp
};

struct MsgVoteReq {
	MsgType mType;
	int 	mTermId;
	int 	mFromSvrId;
};

struct MsgVote{
	MsgType mType;
	int 	mTermId;
	int 	mFromSvrId;
};

struct MsgHeartbeat{
	MsgType mType;
	int 	mTermId;
	int 	mFromSvrId;
	int 	mLeaderSvrId;
	bool 	SvrActiveMap[60];

};

struct MsgHeartbeatResp{
	MsgType mType;
	int 	mTermId;
	int 	mFromSvrId;
};


struct Peer {
	enum Status {
		eUp,
		eDown
	};
	int  mHeartbeatCount;
	Status mStatus;
	udp::endpoint mEp;
	int mServerId;
};

struct Node {
	enum Role {
		eLeader,
		eFollower,	
		eCandidate1,
		eCandidate2
	};

	Role mRole;
	int mTermId;
	int mVoteNum;
	int mHeartbeatTime;
	int mServerId;
	int mLeaderSvrId;
	vector<Peer> mPeers;
	int mNumPeers;
	udp::endpoint mEp;
};

Node self;
udp::endpoint from_ep;
boost::asio::io_service io_service;
udp::socket udpSocket(io_service);
boost::asio::high_resolution_timer timer(io_service);
bool *activeMap;

int rate = 10;
int switchCandidateTimer = 200*rate;
int electionTimer = 200*rate;
int sendHeartbeatTimer = 50*rate;
int checkHeartbeatTimer = 500*rate;

char buff_recv[1024];
char buff_send[1024];


void handleRead(boost::system::error_code ec, std::size_t bytes_recvd);
void handleWrite(boost::system::error_code ec, std::size_t bytes_recvd);
void election(boost::system::error_code ec);
void electionTimeout(boost::system::error_code ec);
void sendHeartbeat(boost::system::error_code ec);
void switchCandidate(boost::system::error_code ec);

string msgType2Str(MsgType type)
{
	switch (type)
	{
		case eVoteReq:
			 return "VR";
		case eVote:
			 return "VT";
		case eHeartbeat:
			 return "HB";
		case eHeartbeatResp:
			 return "HR";
	}
	return "NULL";
}

string roleType2Str(Node::Role role)
{
	switch (role)
	{
		case Node::eLeader:
			 return "LD";
		case Node::eFollower:	
			 return "FL";
		case Node::eCandidate1:
			 return "C1";
		case Node::eCandidate2:
			 return "C2";
	};
	return "NULL";
}

#define LogRaft BOOST_LOG_TRIVIAL(info) << __LINE__ <<  " "  \
					<< "raft:[" << roleType2Str(self.mRole) << "," << self.mTermId << "," << self.mLeaderSvrId << "]" 

#define LogRecvMsg BOOST_LOG_TRIVIAL(info) << __LINE__ <<  " "  \
					<< "recv msg: " << msg.mFromSvrId << "->" \
					<< self.mServerId \
					<< " [" << msgType2Str(msg.mType) << "," << msg.mTermId << "]"

#define LogSendMsg(xmsg, toSvrId) BOOST_LOG_TRIVIAL(info) << __LINE__ <<  " "  \
					<< "send msg: " << self.mServerId << "->" << toSvrId \
					<< " [" << msgType2Str(xmsg.mType) << "," << xmsg.mTermId << "]"

void handleWrite(boost::system::error_code ec, std::size_t bytes_recvd) {
	BOOST_LOG_TRIVIAL(trace) << __func__  << " " << bytes_recvd; 
}

void handleRead(boost::system::error_code ec, std::size_t bytes_recvd) {
	BOOST_LOG_TRIVIAL(trace) << __func__  << " " << bytes_recvd; 
	MsgType type = *(MsgType*)buff_recv;
	switch(type) 
	{
	case eHeartbeat :
		 {
			 MsgHeartbeat& msg = *(MsgHeartbeat*)buff_recv;
			 LogRecvMsg;
			 switch (self.mRole)
			 {
				 case Node::eFollower:
					 if (msg.mTermId >= self.mTermId)
					 {
			 			 MsgHeartbeatResp& sendMsg = *(MsgHeartbeatResp*)buff_send;
						 sendMsg.mType = eHeartbeatResp;
						 sendMsg.mTermId = msg.mTermId;
						 sendMsg.mFromSvrId = self.mServerId;
						 udpSocket.async_send_to(
								boost::asio::buffer(buff_send, sizeof(MsgVote)), from_ep, boost::bind(
									&handleWrite, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
						 LogSendMsg(sendMsg, msg.mFromSvrId);

						 timer.expires_from_now(chrono::milliseconds(switchCandidateTimer));
						 timer.async_wait(boost::bind(&switchCandidate, boost::asio::placeholders::error));;
					 }
				 break;

				 case Node::eCandidate1:
				 case Node::eCandidate2:
					 if (msg.mTermId >= self.mTermId)
					 {
						 self.mRole = Node::eFollower;
						 self.mTermId = msg.mTermId;
						 self.mLeaderSvrId = msg.mLeaderSvrId;
						 LogRaft;

			 			 MsgHeartbeatResp& sendMsg = *(MsgHeartbeatResp*)buff_send;
						 sendMsg.mType = eHeartbeatResp;
						 sendMsg.mTermId = msg.mTermId;
						 sendMsg.mFromSvrId = self.mServerId;
						 udpSocket.async_send_to(
								boost::asio::buffer(buff_send, sizeof(MsgVote)), from_ep, boost::bind(
									&handleWrite, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
						 LogSendMsg(sendMsg, msg.mFromSvrId);

						 timer.expires_from_now(chrono::milliseconds(switchCandidateTimer));
						 timer.async_wait(boost::bind(&switchCandidate, boost::asio::placeholders::error));;
					 }
					 break;
				 case Node::eLeader:
					 if (msg.mTermId > self.mTermId)
					 {
						 self.mRole = Node::eFollower;
						 self.mTermId = msg.mTermId;
						 self.mLeaderSvrId = msg.mLeaderSvrId;
						 LogRaft;

			 			 MsgHeartbeatResp& sendMsg = *(MsgHeartbeatResp*)buff_send;
						 sendMsg.mType = eHeartbeatResp;
						 sendMsg.mTermId = msg.mTermId;
						 sendMsg.mFromSvrId = self.mServerId;
						 udpSocket.async_send_to(
								boost::asio::buffer(buff_send, sizeof(MsgVote)), from_ep, boost::bind(
									&handleWrite, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
						 LogSendMsg(sendMsg, msg.mFromSvrId);

						 timer.expires_from_now(chrono::milliseconds(switchCandidateTimer));
						 timer.async_wait(boost::bind(&switchCandidate, boost::asio::placeholders::error));;
					 }
					 break;
				 default:
					 break;
			 }
		 }
		 break;
		 
	case eVoteReq:
		 {
			 MsgVoteReq& msg = *(MsgVoteReq*)buff_recv;
			 LogRecvMsg;
			 switch (self.mRole)
			 {
				 case Node::eFollower:
				 case Node::eCandidate2:
				 case Node::eLeader:
					 if (msg.mTermId > self.mTermId)
					 {
						 self.mRole = Node::eCandidate2;
						 self.mTermId = msg.mTermId;
						 LogRaft;

			 			 MsgVote& sendMsg = *(MsgVote*)buff_send;
						 sendMsg.mType = eVote;
						 sendMsg.mTermId = msg.mTermId;
						 sendMsg.mFromSvrId = self.mServerId;
						 udpSocket.async_send_to(
								boost::asio::buffer(buff_send, sizeof(MsgVote)), from_ep, boost::bind(
									&handleWrite, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
						 LogSendMsg(sendMsg, msg.mFromSvrId);

						 timer.expires_from_now(chrono::milliseconds(electionTimer));
						 timer.async_wait(boost::bind(&electionTimeout, boost::asio::placeholders::error));
					 }
				 break;

				 case Node::eCandidate1:
					 if (msg.mTermId>= self.mTermId)
					 {
						 self.mRole = Node::eCandidate2;
						 self.mTermId = msg.mTermId;
						 LogRaft;

			 			 MsgVote& sendMsg = *(MsgVote*)buff_send;
						 sendMsg.mType = eVote;
						 sendMsg.mTermId = msg.mTermId;
						 sendMsg.mFromSvrId = self.mServerId;
						 udpSocket.async_send_to(
								boost::asio::buffer(buff_send, sizeof(MsgVote)), from_ep, boost::bind(
									&handleWrite, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
						 LogSendMsg(sendMsg, msg.mFromSvrId);

						 timer.expires_from_now(chrono::milliseconds(electionTimer));
						 timer.async_wait(boost::bind(&electionTimeout, boost::asio::placeholders::error));
					 }
					 break;
				 default:
					 break;
			 }
		 }
		 break;

	case eVote:
		 {
			 MsgVote& msg = *(MsgVote*)buff_recv;
			 LogRecvMsg;
			 switch (self.mRole)
			 {
			 case Node::eCandidate2:
				 if (msg.mTermId == self.mTermId)
				 {
					 self.mVoteNum++;
					 if (self.mVoteNum>self.mNumPeers/2)
					 {
						 self.mRole = Node::eLeader;
						 self.mLeaderSvrId = self.mServerId;
						 LogRaft;
						 self.mVoteNum=0;
						 self.mHeartbeatTime = 0;
						 for (int i=0; i<self.mNumPeers; i++)
						 {
							 self.mPeers[i].mHeartbeatCount = 0;
						 }
						 sendHeartbeat(boost::system::error_code());
					 }
				 }
			 }
		 }
		 break;

	case eHeartbeatResp:
		 {
			 MsgHeartbeatResp& msg = *(MsgHeartbeatResp*)buff_recv;
			 LogRecvMsg;
			 switch (self.mRole)
			 {
			 case Node::eLeader:
				 if (msg.mTermId == self.mTermId)
				 {
					 int idx = msg.mFromSvrId;
				 	 if (msg.mFromSvrId > self.mServerId)
						 idx = idx - 1;
					 self.mPeers[idx].mHeartbeatCount++;
//BOOST_LOG_TRIVIAL(debug) << "XX0 serverid: " << self.mPeers[idx].mServerId << " count: " << self.mPeers[idx].mHeartbeatCount << ", status: " << self.mPeers[idx].mStatus << ", num:" << self.mHeartbeatTime;
				 }
			 }
		 }
	}
	udpSocket.async_receive_from(
		boost::asio::buffer(buff_recv, 1024), from_ep, boost::bind(&handleRead, 
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void sendHeartbeat(boost::system::error_code ec)
{
	BOOST_LOG_TRIVIAL(trace) << __func__ << ec;
	if (ec == 0)
	{
		MsgHeartbeat& msg = *(MsgHeartbeat*)buff_send;
		msg.mType = eHeartbeat;
		msg.mTermId = self.mTermId;
		msg.mFromSvrId = self.mServerId;
		msg.mLeaderSvrId = self.mServerId;
		for (int i=0; i<self.mNumPeers; i++) {
			udpSocket.async_send_to(
					boost::asio::buffer(buff_send, sizeof(MsgHeartbeat)), self.mPeers[i].mEp, boost::bind(
						&handleWrite, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			LogSendMsg(msg, self.mPeers[i].mServerId);
		}

		self.mHeartbeatTime++;

		if (self.mHeartbeatTime == 10)
		{
			for (int i=0; i<self.mNumPeers; i++) {
				int count = 10 - self.mPeers[i].mHeartbeatCount;
				//BOOST_LOG_TRIVIAL(debug) << "XX1 serverid: " << self.mPeers[i].mServerId << " count: " << count << ", status: " << self.mPeers[i].mStatus;
				if (self.mPeers[i].mStatus == Peer::eDown) {
					 if (count < 5)
					 {
						 cout << "Server up" <<  self.mPeers[i].mServerId << endl;
						 self.mPeers[i].mStatus = Peer::eUp;
					 }
				}
				else if (self.mPeers[i].mStatus == Peer::eUp)
				{
					 if (count >= 5)
					 {
						 cout << "Server down" <<  self.mPeers[i].mServerId << endl;
						 self.mPeers[i].mStatus = Peer::eDown;
					 }
				}
			}	
			self.mVoteNum=0;
			self.mHeartbeatTime = 0;
			for (int i=0; i<self.mNumPeers; i++)
			{
				self.mPeers[i].mHeartbeatCount = 0;
			}
		}
		timer.expires_from_now(chrono::milliseconds(sendHeartbeatTimer));
		timer.async_wait(boost::bind(&sendHeartbeat, boost::asio::placeholders::error));
	}
}

void electionTimeout(boost::system::error_code ec)
{
	BOOST_LOG_TRIVIAL(trace) << __func__ << ec;
	if (ec == 0) {
		switchCandidate(ec);
	}
}


void election(boost::system::error_code ec)
{
	BOOST_LOG_TRIVIAL(trace) << __func__ << ec;
	if (ec == 0) {
		self.mRole = Node::eCandidate2;
		self.mTermId++;
		LogRaft;
		self.mVoteNum = 1;

		MsgVoteReq& msg = *(MsgVoteReq*)buff_send;
		msg.mType = eVoteReq;
		msg.mTermId = self.mTermId;
		msg.mFromSvrId = self.mServerId;
		for (int i=0; i<self.mNumPeers; i++) {
			udpSocket.async_send_to(
					boost::asio::buffer(buff_send, sizeof(MsgVoteReq)), self.mPeers[i].mEp, boost::bind(
						&handleWrite, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			LogSendMsg(msg, self.mPeers[i].mServerId);
		}

		timer.expires_from_now(chrono::milliseconds(electionTimer));
		timer.async_wait(boost::bind(&electionTimeout, boost::asio::placeholders::error));
	}
}

void switchCandidate(boost::system::error_code ec)
{
	BOOST_LOG_TRIVIAL(trace) << __func__ << ec;
	if (ec == 0) {
		self.mRole = Node::eCandidate1;
		LogRaft;
		int time = random()%50*rate + 50*rate;
		timer.expires_from_now(chrono::milliseconds(time));
		timer.async_wait(boost::bind(&election, boost::asio::placeholders::error));;
	}
}

void init()
{
	boost::log::core::get()->set_filter
	(
		 boost::log::trivial::severity >= boost::log::trivial::debug
	);
}

int main(int argc, char** argv) {
	if (argc !=2 )
		exit(0);
	
	init();

	int svrid = atoi(argv[1]);

	self.mRole = Node::eFollower;
	self.mTermId = 0;
	self.mVoteNum = 0;
	self.mHeartbeatTime  = 0;
	self.mServerId = svrid;
	self.mLeaderSvrId = -1;
	int port = 11000;
	for (int i=0; i<3; i++)
	{
		if (i == svrid)
		{
			self.mEp = udp::endpoint(address_v4::from_string("192.168.99.184"), port+i);
		}
		else
		{
			Peer p;
			p.mHeartbeatCount = 0;
			p.mStatus = Peer::eDown;
			p.mEp = udp::endpoint(address_v4::from_string("192.168.99.184"), port+i);
			p.mServerId = i;
			self.mPeers.push_back(p);
		}
	}
	self.mNumPeers = self.mPeers.size();

	udpSocket.open(boost::asio::ip::udp::v4());
	udpSocket.bind(self.mEp);
	BOOST_LOG_TRIVIAL(info) << "start raft server serverid: " << self.mServerId << " ep: " 
		<< udpSocket.local_endpoint().address().to_string() << ":" << udpSocket.local_endpoint().port();
	LogRaft;

	udpSocket.async_receive_from(
		boost::asio::buffer(buff_recv, 1024), from_ep, boost::bind(&handleRead, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

	timer.expires_from_now(chrono::milliseconds(switchCandidateTimer));
	timer.async_wait(boost::bind(&switchCandidate, boost::asio::placeholders::error));;

	io_service.run();
	return true;
}
