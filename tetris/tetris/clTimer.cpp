#include "clTimer.h"
#include <cstdio>
#include "tetrisUtility.h"

clTimer *clTimer::_inst = nullptr;
char clTimer::_time_format[30];

clTimer::clTimer(){

    _cur_play_time_milli=0 ;
	_cur_play_time_dp_tl = {0,0};
    on_off_flag=false ;

	memcpy(_time_format, "Time: %d : %d : %d : %d", sizeof(char) * sizeof(clTimer::_time_format));
}

void clTimer::_draw(){

    int hour = _cur_play_time_milli/(3600000) ;
    int min = (_cur_play_time_milli/60000)-hour*60 ;
    int sec = (_cur_play_time_milli/1000)-3600*hour-60*min ;
	int milli = (_cur_play_time_milli % 1000)/10;

    sprintf_s(_time_text,clTimer::_time_format, hour,min,sec,milli) ;

	drawXY(_cur_play_time_dp_tl.X, _cur_play_time_dp_tl.Y, _time_text);

}

void clTimer::_erase() {

	drawXY(_cur_play_time_dp_tl.X, _cur_play_time_dp_tl.Y, _time_text);

}

void clTimer::setStartTime(long long elapsed_time_milli) {

	_cur_play_time_milli = elapsed_time_milli;
}

long long clTimer::getCurrentTimeMilli() {
	
	return _cur_play_time_milli;
}

void _tick(clTimer &timer)
{
    int mill_cnt=0 ;

    while(timer.on_off_flag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)) ;

		timer._cur_play_time_milli++ ;
		timer._erase();
		timer._draw() ;
       
    }
}

void clTimer::start(){

    on_off_flag=true ;
    //start asynchronous timer thread for this object
    tick_event = std::async(std::launch::async,_tick,std::ref(*this));

}

void clTimer::stop(){

    on_off_flag=false ;

    tick_event.wait() ;
}

void clTimer::reset() {

	_cur_play_time_milli = 0;
}

void clTimer::setDpTopLeft(COORD dp_top_left)
{
    _cur_play_time_dp_tl = dp_top_left ;
}

clTimer::~clTimer() {

	stop();
}

clTimer* clTimer::getInstance() {

	if (_inst == nullptr)
	{
		static clTimer timer;

		_inst = &timer;
	}

	return _inst;
}