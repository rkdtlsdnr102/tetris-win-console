#include <Windows.h>
#include <future>

class clTimer{

public:
	struct stDayTime {

		int cur_hour;
		int cur_min;
		int cur_sec;
		int cur_millisec;
	};

private:
    //current time in millisecond, shared with tick_event
	long long _cur_play_time_milli ;
    COORD _cur_play_time_dp_tl ;

    std::future<void> tick_event ;
    bool on_off_flag ;

    friend void _tick(clTimer &timer) ;
	void _draw();
	void _erase();
	clTimer();

	char _time_text[30];

	static clTimer *_inst;
	static const char *_time_format;

public:
    
    void start() ;
    void stop() ;
	void reset();
	void setStartTime(long long elapsed_time_milli);
	long long getCurrentTimeMilli();
    void setDpTopLeft(COORD dp_top_left) ;
	~clTimer();

	static stDayTime getTime(long long cur_millis);
	static clTimer *getInstance();
	

};