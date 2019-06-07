import sys
sys.path.insert(0, "/home/michal/programming/go/libs_python")
import pygo


class RLMatch:

    def __init__(self, experiment_config_file_name, rl_player_file_name, reference_player_file_name):

        self.experiment_config_file_name    = experiment_config_file_name
        self.rl_player_file_name            = rl_player_file_name
        self.reference_player_file_name     = reference_player_file_name

        self.rl_config = pygo.GoRLConfig(experiment_config_file_name)

    def process(self):

        rl_trial_log            = pygo.Log(self.rl_config.result_dir()+"rl_trial_result.log")
        rl_trial_verbose_log    = pygo.Log(self.rl_config.result_dir()+"rl_trial_verbose_result.log")

        reference_trial_log         = pygo.Log(self.rl_config.result_dir()+"referance_trial_result.log")
        reference_trial_verbose_log = pygo.Log(self.rl_config.result_dir()+"referance_trial_verbose_result.log")

        player_a = pygo.GoRLPlayer(self.rl_player_file_name, self.rl_config.board_size())
        player_b = pygo.GoRLPlayer(self.rl_player_file_name, self.rl_config.board_size())

        player_reference = pygo.GoNNPlayer(self.reference_player_file_name, self.rl_config.board_size())

        for trial in range(0, self.rl_config.trials_count()):
            player_a.new_batch()
            player_b.new_batch()

            rl_trial = pygo.GoTrial()

            rl_trial.process(player_a, player_b, self.rl_config.games_count(), self.rl_config.board_size(), self.rl_config.komi(), self.rl_config.visualisation())
            rl_result = rl_trial.get()

            rl_trial_log.put_string(rl_trial.asString())
            rl_trial_log.put_string("\n")
            rl_trial_verbose_log.put_string(rl_trial.asString(True))
            rl_trial_verbose_log.put_string("\n")

            #TODO
            '''
            1, find best player : player_a or player_b
            2, process test() and get_compare_result() -> there is still some bug, too good results
            3, if better than 5% save best player
            3, learn best player
            4, trial with reference supervised player
            5, load player_a and player_b from best saved player
            '''
            
            player_a.test()
            player_a.get_compare_result().save_text_file(self.rl_config.result_dir()+"dqn/result_dqn_")
            player_a.get_compare_result().save_json_file(self.rl_config.result_dir()+"dqn/result_dqn.json")

            player_a.learn()

            reference_trial = pygo.GoTrial()

            reference_trial.process(player_a, player_reference, self.rl_config.games_count(), self.rl_config.board_size(), self.rl_config.komi(), self.rl_config.visualisation())

            reference_trial_log.put_string(reference_trial.asString())
            reference_trial_log.put_string("\n")
            reference_trial_verbose_log.put_string(reference_trial.asString(True))
            reference_trial_verbose_log.put_string("\n");


match = RLMatch("9_9_go/experiment_0/rl_config.json", "9_9_go/experiment_0/rl_player.json", "9_9_go/experiment_0/reference_player.json")
match.process()
print("\nprogram done\n")
