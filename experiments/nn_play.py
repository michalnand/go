import sys
sys.path.insert(0, "/home/michal/programming/go/libs_python")
import pygo


'''
player_a_file_name = "networks/19_19_go/player_2.json"
player_b_file_name = "networks/19_19_go/player_2.json"

board_size = 19

player_a = pygo.GoNNPlayer(player_a_file_name, board_size)
player_b = pygo.GoNNPlayer(player_b_file_name, board_size)

trial = pygo.GoTrial()

trial.process(player_a, player_b, 1000, board_size, 6.5, True)
trial._print()
'''


player_a_file_name = "networks/9_9_go/player_0.json"
player_b_file_name = "networks/9_9_go/player_1.json"

board_size = 9

player_a = pygo.GoNNPlayer(player_a_file_name, board_size)
player_b = pygo.GoNNPlayer(player_b_file_name, board_size)

trial = pygo.GoTrial()

trial.process(player_a, player_b, 1000, board_size, 6.5, True)
trial._print()
