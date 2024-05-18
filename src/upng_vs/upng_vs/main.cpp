#include "ImageProcessing.h"
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;

#pragma warning(disable:4996)

int main() {
	/*<==FAST IO==>*/
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	/*<==Commands==>*/
	const char *commands =
		"\nCommands :               "
		"\n\t-settings              //Shows the settings"
		"\n\t-settings -s           //Set setting manually"
		"\n\t-settings -a		    //Set the settings automatically from videos in <dir>"
		"\n\t-edit					//Edits the videos/images in <dir>\n\n";
	cout << commands << endl;

	while (true)
	{

		cout << ">>";
		std::string command;
		std::getline(std::cin, command);
		
		if (command.substr(0, 4) == "edit")
		{

			/*Get file path*/
			std::string _path_ = command.substr(5, command.length());
			cout << endl;

			auto startTimer = std::chrono::high_resolution_clock::now();
			int count = 0;


			std::filesystem::recursive_directory_iterator start(_path_), end;
			for (; start != end; ++start) {
				auto path = start->path();
				auto full_path = path;
				string finalpath = full_path.generic_string();
				if (finalpath.substr(finalpath.length() - 4) == ".png") 
				{
					ImageProcessing::EditImage(finalpath);
					count++;
				}
			}

			auto elapsed = std::chrono::high_resolution_clock::now() - startTimer;
			long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

			printf("\n\nTotal time of execution(with decoding/encoding) : ");
			cout << milliseconds << "ms" << endl;
			printf("Images analyzed %i images over a time of %ims \n\t=> %fs per image\n\n",count, milliseconds,(milliseconds/count)/1000.0);
			
		}

	}

	std::getchar();
	return 0;

}
