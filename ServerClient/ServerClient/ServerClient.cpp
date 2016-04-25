// ServerClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//int main()
//{
//    return 0;
//}
// ConsoleApplication21.cpp : Defines the entry point for the console application.
//
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/http_client.h>

using namespace web::http::client;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

#include <iostream>
#include <map>
#include <set>
#include <string>
using namespace std;

#define TRACE(msg)            wcout << msg
#define TRACE_ACTION(a, k, v) wcout << a << L" (" << k << L", " << v << L")\n"
string s = "sri";
map<utility::string_t, utility::string_t> dictionary;
vector<utility::string_t> values; //holds  a set of string values in the server for test.

//helper method to add string_t members to the string vector.
void push(string s)
{
	values.push_back(utility::conversions::to_string_t(s));
}

auto ss=utility::conversions::to_string_t(s);
/* handlers implementation */
//TEST: Get Dictionary Operations
void handle_get(http_request request)
{
	TRACE(L"\nhandle GET\n");

	json::value answer;
	auto i = 0;
	for (auto const & p : dictionary)
	{
		answer[i] = json::value(p.first);
		answer[i + 1] = json::value(p.second);
		i += 2;
	}

	request.reply(status_codes::OK, answer);
}

//TEST: Get Dictionary Operations
void handle_gets(http_request request)
{
	TRACE(L"\nhandle GET\n");
	
	json::value answer;
	json::value ans;


	auto i = 0;
	for (auto const & p : dictionary)
	{
		answer[i]=(json::value(p.first));
		answer[i+1]= (json::value(p.second));
		i += 2;
	}

	request.reply(status_codes::OK, answer);
}

//TEST: Returns all values in the test string vector.
void handle_getvector(http_request request)
{
	TRACE(L"\nhandle GET\n");

	json::value answer;
	json::value ans;


	auto i = 0;
	for (auto const & p : values)
	{
		answer[i] = (json::value(p));
		
		i++;
	}

	request.reply(status_codes::OK, answer);
}

//Umbrella method to handle POST, PUT and DELETE operations.
void handle_request(http_request request,
	 function<void(json::value &, json::value &)> action)
{
	json::value answer;
	
	request
		.extract_json()
		.then([&answer, &action](pplx::task<json::value> task) {
		try
		{
			auto & jvalue = task.get();

			if (!jvalue.is_null())
			{
				action(jvalue, answer);
			}
		}
		catch (http_exception const & e)
		{
			wcout << e.what() << endl;
		}
	})
		.wait();

	request.reply(status_codes::OK, answer);
}

//TEST: Checks if a given string is present in the string vector.
void handle_post(http_request request)
{
	TRACE("\nhandle POST\n");

	handle_request(
		request,
		[](json::value & jvalue, json::value &answer)
	{bool found = false;
	std:cout << "True";
	auto s = jvalue.as_string();
	auto str = utility::conversions::to_utf8string(s);
	for (auto i : values)
	{
		if (utility::conversions::to_utf8string(i)==str)
		{
			cout << "Matched" << endl;
			found = true;
			break;
		}
	}
	
		string msg = "Finding " + utility::conversions::to_utf8string(s);
		if(found)
		{
			
			answer = json::value(utility::conversions::to_string_t(msg)+ utility::conversions::to_string_t(": found!"));
		}
		else
		{
			
			answer = json::value(utility::conversions::to_string_t(msg) + utility::conversions::to_string_t(": Not found!"));
		}

	}

	);
}

//LIVE: Performs sentiment analysis of the given tweet wrapped around a JSON object and gives out the result in the form of another JSON object.
void handle_post_sentiment(http_request request)
{
	TRACE("\nhandle POST Sentimetnt Analysis \n");
	handle_request(
		request,
		[](json::value & jvalue, json::value &answer)
	{	
	http_client client(U("http://www.sentiment140.com/api/bulkClassifyJson?appid=sriharisridhar@hotmail.com"));
	http_request req;
	req.set_body(jvalue);
	req.set_method(methods::POST);
	req.headers().set_content_type(U("application/json"));
	client.request(req).then([&](http_response resp) 
	{
		if (resp.status_code() == status_codes::OK)
		{
			cout << "OKAY!" << endl;
			answer = resp.extract_json().get();
		}
		else {
			/* Print bad status code */
			wcout << L"Server returned returned status code " << resp.status_code() << L'.' << std::endl;
		}
	}).wait();

	}

	);

}

//TEST: Adds a new string to the string vector.
void handle_put(http_request request)
{
	TRACE("\nhandle POST\n");

	handle_request(
		request,
		[](json::value & jvalue, json::value &answer)
	{
		auto s = jvalue.as_string();
		values.push_back(s);
		string msg = "Added " + utility::conversions::to_utf8string(s);
		answer = json::value(utility::conversions::to_string_t(msg));
		
	}
	
	);
}
//TEST: Deletes a given String from the test string vector.
void handle_delete(http_request request)
{
	TRACE("\nhandle POST\n");

	handle_request(
		request,
		[](json::value & jvalue, json::value &answer)
	{	bool found = false;
		auto s = jvalue.as_string();
    auto str = utility::conversions::to_utf8string(s);
	auto it = values.begin();
	for (auto i : values)
	{
		if (utility::conversions::to_utf8string(i)==str)
		{
			cout << "Matched" << endl;
			values.erase(it);
			found = true;
			break;
		}
		it++;
	}
	
	string msg = "";

		if(found)
		{
			
			 msg = "Deleted: " + utility::conversions::to_utf8string(s);
		}
		else
		{
			msg = "String not found. Could not delete: " + utility::conversions::to_utf8string(s);

		}

		
		answer = json::value(utility::conversions::to_string_t(msg));

	}

	);
}

int main()
{
	dictionary.emplace(utility::conversions::to_string_t("Name"), utility::conversions::to_string_t("Sri"));
	dictionary.emplace(utility::conversions::to_string_t("Name2"), utility::conversions::to_string_t("Sri2"));
	push("Sri");
	push("sri2");
	http_listener listener(L"http://localhost:4445/restdemo");

	listener.support(methods::GET, handle_getvector);
	listener.support(methods::POST, handle_post_sentiment);
	listener.support(methods::PUT, handle_put);
	listener.support(methods::DEL, handle_delete);

	try
	{
		listener
			.open()
			.then([&listener]() {TRACE(L"\nstarting to listen\n"); })
		.wait();

	while (true);
	}
	catch (exception const & e)
	{
		wcout << e.what() << endl;
	}

	return 0;
}



