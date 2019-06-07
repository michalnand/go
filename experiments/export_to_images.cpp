#include <iostream>

#include <GoDatasetRuntime.h>
#include <image_save.h>

void save_image(unsigned int id, sDatasetItem &item, std::string path)
{
	unsigned int class_id = 0;

	for (unsigned int i = 0; i < item.output.size(); i++)
		if (item.output[i] > item.output[class_id])
			class_id = i;

	unsigned int width = 9;
	unsigned int height = 9;

	unsigned int size = width*height;

	std::vector<float> vect(size*3);
	unsigned int ptr = 0;

	for (unsigned int i = 0; i < size; i++)
	{
		vect[ptr] = item.input[i + size*0];
		ptr++;
	}
	for (unsigned int i = 0; i < size; i++)
	{
		vect[ptr] = item.input[i + size*1];
		ptr++;
	}
	for (unsigned int i = 0; i < size; i++)
	{
		vect[ptr] = item.input[i + size*3];
		ptr++;
	}

	std::string file_name = path + std::to_string(class_id) + "/" + std::to_string(id) + ".png";


	ImageSave image(width, height, false);

	image.save(file_name, vect);
}

int main()
{

	srand(time(NULL));

	std::string dataset_file_name = "networks/9_9_go/dataset_config.json";
 	GoDatasetRuntime dataset(dataset_file_name);
	dataset.create();

    unsigned int training_count = 2500000;
	unsigned int testing_count 	= dataset.get_testing_size();

	for (unsigned int j = 0; j < testing_count; j++)
    {
        sDatasetItem item = dataset.get_testing(j);
        save_image(j, item, "/home/michal/dataset/go_9_9_images/testing/");

		if ((j%1000) == 0)
			std::cout << "export testing done " << j*100.0/testing_count << "\n";
    }

    for (unsigned int j = 0; j < training_count; j++)
    {
        sDatasetItem item = dataset.get_random_training();
        save_image(j, item, "/home/michal/dataset/go_9_9_images/training/");

		if ((j%1000) == 0)
			std::cout << "export training done " << j*100.0/training_count << "\n";
    }

	std::cout << "program done\n";

 	return 0;
}
