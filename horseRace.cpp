#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "utility/Random.h"

constexpr auto horse_count { 6 };
constexpr std::string_view colours [horse_count] = {
	"\033[38;5;9m",
	"\033[38;5;10m",
	"\033[38;5;11m",
	"\033[38;5;12m",
	"\033[38;5;13m",
	"\033[38;5;14m"
};

constexpr int track_length_meters { 100 };
constexpr std::string_view horsey { "h" };

void moveCursorTo(int row, int col)
{
	printf("\033[%d;%dH", row, col);
}

void clearRowText(int row)
{
	moveCursorTo(row, 1);
	printf("\033[K");
}

void replaceTextAtCursor(std::string_view r)
{
	printf("\b");
	std::cout << r;
	
}
int pickHorseToMove()
{
	return Random::get( 0 , horse_count - 1 );
}

void initTrack()
{
	moveCursorTo(1, 1);
	std::cout << "\n";

	for( int i { 0 }; i < horse_count; ++i )
	{
		for( int j { 0 }; j < track_length_meters; ++j )
		{
			std::cout << "-";
		}
		std::cout << "\n";
	}
	
}

template <typename T>
T horseInTheLead(const T positions [horse_count] )
{
	auto max_index  { 0 };
	for ( int i { 1 }; i < horse_count; ++i )
	{
		if (positions[i] > positions[max_index]) {
			max_index = i;
		}
	}
	return max_index;

}

const char* horseInTheLead(const char* positions [horse_count] ) = delete;

void updateTrack(const int positions [horse_count], const int moving_horse)
{
	const int in_the_lead = horseInTheLead<int>(positions);

	if ( positions[in_the_lead] == 0 ) {
		std::cout << colours[moving_horse];
		clearRowText(1);
		std::cout << "Horse " << moving_horse + 1<< " is in the lead!";
	}
	else if ( positions[moving_horse] >= positions[in_the_lead] )
	{
		std::cout << colours[in_the_lead];
		clearRowText(1);
		std::cout << "Horse " << in_the_lead + 1<< " is in the lead!";
	}
	if ( positions[in_the_lead] >= 100) {
		std::cout << colours[in_the_lead];
		clearRowText(1);
		std::cout << "Horse " << in_the_lead + 1<< " has won the race!";
	}
	
	std::cout << colours[moving_horse];
	moveCursorTo(moving_horse + 2, positions[moving_horse] + 1);
	replaceTextAtCursor(horsey);
	// so long as they arent on the first position (remember ansi starts on 1!) remove old the horse
	if( positions[moving_horse] > 1 )
	{
		moveCursorTo(moving_horse + 2, positions[moving_horse] );
		replaceTextAtCursor("-");
	}
	
}

int main()
{
	std::cout << std::unitbuf;
	int positions [horse_count] = { 0, 0, 0, 0, 0, 0 };

	bool race_over = false;

	// Clear the screen
	printf("\033[2J");
	initTrack();
	while(! race_over)
	{
		int moving_horse = pickHorseToMove();
		++positions[moving_horse];
		if ( positions[moving_horse] >=100)
		{
			race_over = true;
		}
		updateTrack(positions, moving_horse);
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
	moveCursorTo(horse_count + 2, 1);
	return 0;
}

