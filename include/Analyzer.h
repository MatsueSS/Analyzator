#ifndef _ANALYZER_H_
#define _ANALYZER_H_

class Analyzer{
public:
    Analyzer();

    void start_process();

private:
    //start a process for time control
    void time_check();
};

#endif //_ANALYZER_H_