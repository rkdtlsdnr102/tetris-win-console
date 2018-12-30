#include "clTimer.h"
#include <cstdio>
#include "tetrisUtility.h"

#ifdef _DEBUG
#include <iostream>
#endif 

clTimer *clTimer::_inst = nullptr;
const char *clTimer::_time_format = (const char*)"Time: %d : %d : %d : %d";

clTimer::clTimer(){

    _cur_play_time_milli=0 ;
	_cur_play_time_dp_tl = {0,0};
    on_off_flag=false ;
}

void clTimer::_draw(){

	stDayTime time = getTime(_cur_play_time_milli);

	sprintf(_time_text, clTimer::_time_format, time.cur_hour, time.cur_min, time.cur_sec, time.cur_millisec); ;

	drawXY(_cur_play_time_dp_tl.X, _cur_play_time_dp_tl.Y, _time_text);

}

void clTimer::_erase() {

	static const char time_erase_text[31] = { ' ', ' ',' ',' ',' ', ' ',' ',' ',' ',' ',
											' ', ' ',' ',' ',' ', ' ',' ',' ',' ',' ', 
											' ', ' ',' ',' ',' ', ' ',' ',' ',' ',' ', };


	drawXY(_cur_play_time_dp_tl.X, _cur_play_time_dp_tl.Y, time_erase_text);

}



clTimer::stDayTime clTimer::getTime(long long cur_millis)
{
	stDayTime time;

	time.cur_hour = cur_millis / (3600000);
	time.cur_min = (cur_millis / 60000) - time.cur_hour * 60;
	time.cur_sec = (cur_millis / 1000) - 3600 * time.cur_hour - 60 * time.cur_min;
	time.cur_millisec = (cur_millis % 1000) / 10;

	return time;

}

void clTimer::setStartTime(long long elapsed_time_milli) {
	
	_cur_play_time_milli = elapsed_time_milli;
}

long long clTimer::getCurrentTimeMilli() {
	
	return _cur_play_time_milli;
}

void _tick(clTimer &timer)
{
	timer._erase();

    while(timer.on_off_flag)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(19)) ;

		timer._cur_play_time_milli++ ;
		
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

	if(tick_event.valid())
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

#ifdef _DEBUG
	std::cout << "Timer Stopped\n";
#endif
	
}

clTimer* clTimer::getInstance() {

	if (_inst == nullptr)
	{
		static clTimer timer;

		_inst = &timer;
	}

	return _inst;
}