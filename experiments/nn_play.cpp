#include <iostream>
#include <GoNNPlayer.h>
#include <GoTrial.h>

int main()
{
	srand(time(NULL));

	std::string player_a_file_name = "networks/9_9_go/player_0.json";
	std::string player_b_file_name = "networks/9_9_go/player_0.json";
	unsigned int board_size = 9;

	GoNNPlayer player_a(player_a_file_name, board_size);
	GoNNPlayer player_b(player_b_file_name, board_size);

	GoTrial trial;

	trial.process(player_a, player_b, 100, board_size, 6.5, true);
	trial.print();

	std::cout << "program done\n";

 	return 0;
}
