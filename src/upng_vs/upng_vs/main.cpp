#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <lodepng.h>

#include "Color.h";

using namespace std;

#pragma warning(disable:4996)

/*SETTINGS*/
bool fileExists(const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

struct Settings
{
public:
	/*
	Index - Property
	0 - Color Balance
	1 - White Intensity
	2 - Black Intensity
	3 - Shadow Intensity
	4 - Vividity Min
	5 - Vividity Max
	*/
	float data[6];

	Settings(string path)
	{
		std::ifstream file(path);
		std::string str;
		int k = 0;
		while (std::getline(file, str)) {
			data[k] = std::stof(str);
			k++;
		}
	}
	Settings() { memset(data, -1, sizeof(data)); }

	void DisplaySettings()
	{
		cout << "\nColor Balance : " << data[0] << endl;
		cout << "White Intensity : " << data[1] << endl;
		cout << "Black Intensity : " << data[2] << endl;
		cout << "Shadow Intensity : " << data[3] << endl;
		cout << "Vividity Min : " << data[4] << endl;
		cout << "Vividity Max : " << data[5] << endl << endl;
	}
};
/*SETTINGS END*/


/*IMAGES*/
std::vector<unsigned char> decodeImage(const char* filename) {
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	return image;
}

void encodeImage(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
	unsigned error = lodepng::encode(filename, image, width, height);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}
/*IMAGES END*/




Settings currentSettings = Settings("settings.ini");
int main() {
	/*<==FAST IO==>*/
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	/*<==TITLE==>*/
	const char *title =
		"     ----     `---.      .---.       .---.      .---`       `-----..`            `--------.            .://:-`          "
		"    `MMMM`    -MMMm      -NMMM:     -NMMM:      hMMM/       oMMMMMMMMNh/`        :MMMMMMMMd         /hNMMMMMMMms-       "
		"    `MMMM`    -MMMm       :MMMN.   `mMMM+       hMMM/       oMMMmssymMMMN:       :MMMNssss+       .dMMMMmhyhNMMMMy      "
		"    `MMMM`    -MMMm        oMMMd`  hMMMs        hMMM/       oMMMs    +MMMN.      :MMMm`````      `mMMMh.     :mMMMy     "
		"    `MMMM`    -MMMm         yMMMy oMMMh         hMMM/       oMMMs     mMMM+      :MMMMMMMMo      /MMMN        :MMMM`    "
		"    `MMMM`    :MMMm          dMMMyMMMd`         hMMM/       oMMMs    `NMMM:      :MMMNyyyy/      -MMMM-       oMMMm     "
		"     mMMMs`  .hMMMy          `mMMMMMN.          hMMM/       oMMMs  .+mMMMh       :MMMm            sMMMNs:.`./hMMMM:     "
		"     -NMMMMNNMMMMy`           .NMMMN-           hMMM/       oMMMMMMMMMMm+        :MMMMMMMMd        /mMMMMMMMMMMMh-      "
		"      `/sdmNmmho.              :hhh:            shhh:       /hhhhhhhs+-          -hhhhhhhhs          -ohdmNmdy+.        ";
	cout << title << "\n by Adrian Fluder" << endl;

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

		if (command == "settings")
		{
			if (!fileExists("settings.ini"))
			{
				std::ofstream file("settings.ini");
				file << "1\n100\n0\n50\n50\n70" << std::endl;
				file.close();
				currentSettings = Settings("settings.ini");
			}
			currentSettings.DisplaySettings();
		}
		else if (command == "settings -s")
		{

			string cb, wi, bi, si, vmi, vma;
			cout << "\nColor Balance : " << endl;
			std::getline(std::cin, cb);
			cout << "White Intensity : " << endl;
			std::getline(std::cin, wi);
			cout << "Black Intensity : " << endl;
			std::getline(std::cin, bi);
			cout << "Shadow Intensity : " << endl;
			std::getline(std::cin, si);
			cout << "Vividity Min : " << endl;
			std::getline(std::cin, vmi);
			cout << "Vividity Max : " << endl;
			std::getline(std::cin, vma);
			std::string output = cb + "\n" + wi + "\n" + bi + "\n" + si + "\n" + vmi + "\n" + vma;

			std::ofstream file("settings.ini");
			file << output << std::endl;
			file.close();

			cout << "\nNew Settings" << endl;
			currentSettings = Settings("settings.ini");
			currentSettings.DisplaySettings();
		}
		else if (command == "settings -a")
		{
			cout << "Not implemented!\n\n";
		}
		else if (command == "edit")
		{
			std::string path = "";
			cout << "\File Path : ";
			std::getline(std::cin, path);
			/*Decode*/
			std::vector<unsigned char> image = decodeImage(path.c_str());
			std::vector<unsigned char> *imgPtr = &image;

			float w = 800.0;
			float h = 600.0;
			float imageSize = w * h;

			vector<Color> imageCols(imageSize);

			for (size_t i = 0; i < (imageSize * 4) - 1; i += 4)
			{
				Color col = Color((float)((*imgPtr)[i]), (float)((*imgPtr)[i + 1]), (float)((*imgPtr)[i + 2]), Color::ColorType::RGB);
				imageCols[i / 4] = col;
			}

			vector<vector<Color>> sortedCols(765);

			for (size_t i = 0; i < imageCols.size(); i += 1)
			{
				int index = (int)(imageCols[i].data[0] + imageCols[i].data[1] + imageCols[i].data[2]);
				sortedCols[index].push_back(imageCols[i]);
			}

			int perc03 = round(0.8 / 100.0*imageSize);
			int currCount = 0;
			vector<Color> perc01Cols(perc03);

			Color referenceWhite = Color(0, 0, 0, Color::ColorType::HSV);

			for (int i = 764; i > -1; i--)
			{
				if (currCount < perc03)
				{
					int size = sortedCols[i].size();
					if (size > 0)
					{
						for (int j = 0; j < size; j++)
						{
							if (currCount < perc03) {
								Color col = sortedCols[i][j];
								perc01Cols.push_back(col);
								currCount++;

								col.SwitchTo(Color::ColorType::HSV);

								referenceWhite.data[0] += col.data[0] / perc03;
								referenceWhite.data[1] += col.data[1] / perc03;
								referenceWhite.data[2] += col.data[2] / perc03;

								col.SwitchTo(Color::ColorType::RGB);
							}
							else
							{
								break;
							}
						}
					}
				}
				else
				{
					break;
				}

			}

			printf("\nreference White : \nH:%f S:%f V:%f\n", referenceWhite.data[0], referenceWhite.data[1], referenceWhite.data[2]);
			referenceWhite.SwitchTo(Color::ColorType::RGB);
			printf("\nreference White : \nR:%f G:%f B:%f\n", referenceWhite.data[0], referenceWhite.data[1], referenceWhite.data[2]);

			referenceWhite = Color(1 / (referenceWhite.data[0] / 255.0), 1 / (referenceWhite.data[1] / 255.0), 1 / (referenceWhite.data[2] / 255.0), Color::ColorType::RGB);
			printf("\nreference White : \nR:%f G:%f B:%f\n", referenceWhite.data[0], referenceWhite.data[1], referenceWhite.data[2]);

			for (int i = 0; i < imageSize; i++)
			{
				Color* col;
				col = &imageCols[i];
				(*col).SwitchTo(Color::ColorType::RGB);
				(*col).data[0] *= referenceWhite.data[0];
				(*col).data[1] *= referenceWhite.data[1];
				(*col).data[2] *= referenceWhite.data[2];

				if ((*col).data[0] >= 255)
					(*col).data[0] = 255;

				if ((*col).data[1] >= 255)
					(*col).data[1] = 255;

				if ((*col).data[2] >= 255)
					(*col).data[2] = 255;

				(*imgPtr)[i * 4] = (char)round((*col).data[0]);
				(*imgPtr)[(i * 4) + 1] = (char)round((*col).data[1]);
				(*imgPtr)[(i * 4) + 2] = (char)round((*col).data[2]);
			}

			encodeImage(("updated_" + path).c_str(), *imgPtr, w, h);
		}
	}

	std::getchar();
	return 0;

}

