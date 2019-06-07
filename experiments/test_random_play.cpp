#include <stdio.h>
#include <Goban.h>

#include <stdlib.h>
#include <timer.h>
#include <iostream>


#include <GoSgfPlayer.h>


int main()
{
	srand(time(NULL));

	Timer timer;
	Goban goban(19);

	GoSgfPlayer player("testing_game.sgf");

	int result = 0;
	timer.start();
	for (unsigned int i = 0; i < player.get_moves_count(); i++)
	{
		auto move = player.get_move(i);

		result = goban.play_move(move);
		goban.print();
		goban.render(true);
		timer.sleep_ms(5);
	}
	timer.stop();

	std::cout << "time = " << timer.get_duration() << "\n";
	std::cout << "result = " << result << "\n";



 	return 0;
}
