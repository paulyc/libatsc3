/*
 * output_statistics_ncurses.h
 *
 *  Created on: Feb 7, 2019
 *      Author: jjustman
 *
 *      hacks:
 *
 *
  FILE *f = fopen("/dev/tty", "r+");
  SCREEN *screen = newterm(NULL, f, f);
  set_term(screen);

  //this goes to stdout
  fprintf(stdout, "hello\n");
  //this goes to the console
  fprintf(stderr, "some error\n");
  //this goes to display
  mvprintw(0, 0, "hello ncurses");
  refresh();
  getch();
  endwin();

  return 0;

  #define _DUMP_ALL_MPU_FLOWS_ true

 */


#include <stdarg.h>
#include <ncurses.h>                    /* ncurses.h includes stdio.h */
#include <pthread.h>
#include <stdlib.h>

#include "atsc3_lls.h"

#ifndef ATSC3_OUTPUT_STATISTICS_NCURSES_H_
#define ATSC3_OUTPUT_STATISTICS_NCURSES_H_

pthread_mutex_t ncurses_writer_lock;
void ncurses_mutext_init();

void ncurses_writer_lock_mutex_acquire();
void ncurses_writer_lock_mutex_release();
void ncurses_writer_lock_mutex_destroy();

void lls_dump_instance_table_ncurses(lls_table_t* lls_session);


//#if defined OUTPUT_STATISTICS && OUTPUT_STATISTICS == NCURSES
#define __BW_STATS_NCURSES true
#define __PKT_STATS_NCURSES true


SCREEN* my_screen;
//wmove(bw_window, 0, 0 __VA_ARGS__); //vwprintf(bw_window, __VA_ARGS__);
WINDOW* my_window;

WINDOW* left_window_outline;
	WINDOW* pkt_global_stats_window;
	WINDOW* signaling_global_stats_window;

	WINDOW* bw_window_outline;
		WINDOW* bw_window_runtime;
		WINDOW* bw_window_lifetime;


WINDOW* right_window_outline;
	WINDOW* pkt_flow_stats_window;

WINDOW* bottom_window_outline;
	WINDOW* pkt_global_loss_window;

int global_mmt_loss_count;

#define __DOUPDATE()				doupdate();

#define __BW_STATS_NOUPDATE() 		wnoutrefresh(bw_window_runtime); \
									wnoutrefresh(bw_window_lifetime);

#define __BW_STATS_RUNTIME(...) 	wprintw(bw_window_runtime, __VA_ARGS__); \
									wprintw(bw_window_runtime,"\n");

#define __BW_STATS_LIFETIME(...)	wprintw(bw_window_lifetime, __VA_ARGS__); \
									wprintw(bw_window_lifetime,"\n");

#define __BW_CLEAR() 				werase(bw_window_runtime); \
									werase(bw_window_lifetime); \

#define __PS_STATS_NOUPDATE()		wnoutrefresh(pkt_global_stats_window); \
									wnoutrefresh(pkt_flow_stats_window); \
									wnoutrefresh(pkt_global_loss_window);

#define __PS_STATS_GLOBAL(...) 		wprintw(pkt_global_stats_window, __VA_ARGS__); \
									wprintw(pkt_global_stats_window,"\n");

#define __PS_STATS_FLOW(...) 		wprintw(pkt_flow_stats_window, __VA_ARGS__); \
									wprintw(pkt_flow_stats_window,"\n");

#define __PS_STATS_HR()				whline(pkt_flow_stats_window, ACS_BOARD, 15); \
									wprintw(pkt_flow_stats_window,"\n");



#define __PS_STATS_LOSS(...) 		wprintw(pkt_global_loss_window, __VA_ARGS__); \
									wprintw(pkt_global_loss_window,"\n");

#define __PS_REFRESH_LOSS() 		wrefresh(pkt_global_loss_window);


#define __PS_REFRESH() 				wrefresh(pkt_global_stats_window); \
									wrefresh(pkt_flow_stats_window);

#define __PS_CLEAR() 				werase(pkt_global_stats_window); \
									werase(pkt_flow_stats_window);

#define __PS_STATS_GLOBAL_LOSS(...)	wprintw(pkt_global_loss_window, __VA_ARGS__); \
									wprintw(pkt_global_loss_window,"\n");

#define __PS_STATS_STDOUT(...)		printf(__VA_ARGS__); \
									printf("\n");


#define __LLS_DUMP_NOUPDATE()		wnoutrefresh(signaling_global_stats_window);

#define __LLS_DUMP_CLEAR()			werase(signaling_global_stats_window);

#define __LLS_DUMP(...)				wprintw(signaling_global_stats_window, __VA_ARGS__); \
									wprintw(signaling_global_stats_window,"\n");

#define __LLS_REFRESH()		 		wrefresh(signaling_global_stats_window);



#endif /* ATSC3_OUTPUT_STATISTICS_NCURSES_H_ */