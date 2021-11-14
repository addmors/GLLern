#pragma once
#include <chrono>
#include <iostream>
#include <string>
#include <map>
#include <AntTweakBar.h>

using namespace std;
using namespace std::chrono;


class LogDuration {
    
public:
    static map<string, float> Walues;
    static TwBar* bar;
    static void Init()
    {
        bar = TwNewBar("DurationBar");
        TwDefine("DurationBar size='200 400' color='96 216 224' ");
    };
    
    explicit LogDuration(const string& msg = "")
        : message(msg + ": ")
        , start(steady_clock::now())
    {
    }

    ~LogDuration() {
        auto finish = steady_clock::now();
        auto dur = finish - start;

        if (Walues.find(message) == Walues.end()) {
            Walues[message] = duration_cast<microseconds>(dur).count();

            TwAddVarRW(bar, message.c_str(), TW_TYPE_FLOAT, &Walues[message], NULL);
        }
        else {
            Walues[message] = duration_cast<microseconds>(dur).count();
        }

        cerr << message
            << duration_cast<microseconds>(dur).count()
            << " ns" << endl;
    }
private:
    string message;
    steady_clock::time_point start;
};

#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)

#define LOG_DURATION(message) \
  LogDuration UNIQ_ID(__LINE__){message};
 
