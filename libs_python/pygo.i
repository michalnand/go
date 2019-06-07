%module pygo

%include <std_vector.i>
%include <std_string.i>

%template(VectorChar) std::vector<char>;
%template(VectorUnsignedChar) std::vector<unsigned char>;
%template(VectorInt) std::vector<int>;
%template(VectorUnsignedInt) std::vector<unsigned int>;
%template(VectorFloat) std::vector<float>;
%template(VectorDouble) std::vector<double>;

%template(MatrixInt) std::vector<std::vector<int>>;
%template(MatrixUnsignedInt) std::vector<std::vector<unsigned int>>;

%template(MatrixFloat) std::vector<std::vector<float>>;
%template(MatrixDouble) std::vector<std::vector<double>>;


%apply const std::string& {std::string* foo};



%{
#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>

#include <log.h>

#include <dataset_interface.h>
#include <dataset_binary.h>
#include <dataset_images.h>
#include <dataset_mnist.h>
#include <dataset_pair.h>
#include <dataset_preprocessing.h>
#include <dataset_tic_tac_toe.h>

#include <cnn.h>
#include <histogram.h>
#include <classification_compare.h>
#include <regression_compare.h>
#include <classification_experiment.h>
#include <regression_experiment.h>
#include <autoencoder_experiment.h>



#include <dqn.h>
#include <ddqn.h>
#include <dqn_compare.h>
#include <dqnp.h>
#include <random_distribution.h>

#include <go_defs.h>
#include <GoRLConfig.h>
#include <GoMove.h>
#include <Goban.h>
#include <GoNNInput.h>
#include <GoBoardState.h>
#include <GoDatasetRuntime.h>
#include <GoDatasetValueRuntime.h>

#include <GoPlayer.h>
#include <GoHumanPlayer.h>
#include <GoSgfPlayer.h>
#include <GoNNPlayer.h>
#include <GoRLPlayer.h>

#include <go_supervised.h>
#include <GoTrial.h>
%}

%include <log.h>

%include <dataset_interface.h>
%include <dataset_binary.h>
%include <dataset_images.h>
%include <dataset_mnist.h>
%include <dataset_pair.h>
%include <dataset_preprocessing.h>
%include <dataset_tic_tac_toe.h>

%include <cnn.h>
%include <histogram.h>
%include <classification_compare.h>
%include <regression_compare.h>
%include <classification_experiment.h>
%include <regression_experiment.h>
%include <autoencoder_experiment.h>



%include <dqn.h>
%include <ddqn.h>
%include <dqn_compare.h>
%include <dqnp.h>
%include <random_distribution.h>

%include <go_defs.h>
%include <GoRLConfig.h>
%include <GoMove.h>
%include <Goban.h>
%include <GoNNInput.h>
%include <GoBoardState.h>
%include <GoDatasetRuntime.h>
%include <GoDatasetValueRuntime.h>

%include <GoPlayer.h>
%include <GoHumanPlayer.h>
%include <GoSgfPlayer.h>
%include <GoNNPlayer.h>
%include <GoRLPlayer.h>

%include <go_supervised.h>
%include <GoTrial.h>
