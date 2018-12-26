#include <Windows.h>
#include <future>

class clTimer{

private:
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
	static char _time_format[30];

public:
    
    void start() ;
    void stop() ;
	void reset();
	void setStartTime(long long elapsed_time_milli);
	long long getCurrentTimeMilli();
    void setDpTopLeft(COORD dp_top_left) ;
	~clTimer();


	static clTimer *getInstance();

};