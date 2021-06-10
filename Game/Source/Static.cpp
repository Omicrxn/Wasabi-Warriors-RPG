#include "Static.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Log.h"
#include "Map.h"
#include "EntityManager.h"

Static::Static(std::string name, Textures* tex, EntityManager* entityManager, iPoint position) : Entity(EntityType::STATIC)
{
	this->name.Create(name.c_str());
	this->tex = tex;
	this->entityManager = entityManager;
	this->position = position;

	//Assign type of static entity, texture rect and pivot
	//Orthogonal map ------------------------
	// Town sorting
	if (name == "tree_0")
	{
		type = Static::Type::TREE;
		SetRect(512, 546, 32, 46);
		SetPivot(16, 46);
	}

	if (name == "roof_0")
	{
		type = Static::Type::ROOF;
		SetRect(32, 320, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_1")
	{
		type = Static::Type::ROOF;
		SetRect(224, 544, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_2")
	{
		type = Static::Type::ROOF;
		SetRect(224, 320, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "cup_0")
	{
		type = Static::Type::CUP;
		SetRect(299, 493, 10, 9);
		SetPivot(10, 36);
	}

	if (name == "roof_3")
	{
		type = Static::Type::ROOF;
		SetRect(352, 320, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_4")
	{
		type = Static::Type::ROOF;
		SetRect(320, 544, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_5")
	{
		type = Static::Type::ROOF;
		SetRect(32, 352, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_6")
	{
		type = Static::Type::ROOF;
		SetRect(352, 352, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_7")
	{
		type = Static::Type::ROOF;
		SetRect(160, 640, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_8")
	{
		type = Static::Type::ROOF;
		SetRect(160, 416, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "signe_0")
	{
		type = Static::Type::SIGNE;
		SetRect(614, 706, 15, 28);
		SetPivot(8, 28);
	}

	if (name == "machine_0")
	{
		type = Static::Type::MACHINE;
		SetRect(422, 597, 19, 26);
		SetPivot(10, 26);
	}

	if (name == "roof_9")
	{
		type = Static::Type::ROOF;
		SetRect(320, 576, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "window_0")
	{
		type = Static::Type::WINDOW;
		SetRect(102, 116, 19, 34);
		SetPivot(19, 136);
	}

	if (name == "light_0")
	{
		type == Static::Type::LIGHT;
		SetRect(711, 309, 18, 105);
		SetPivot(9, 105);
	}

	if (name == "bin_0")
	{
		type = Static::Type::BIN;
		SetRect(458, 650, 12, 18);
		SetPivot(6, 18);
	}

	if (name == "bin_1")
	{
		type = Static::Type::BIN;
		SetRect(426, 649, 12, 18);
		SetPivot(6, 18);
	}

	if (name == "roof_10")
	{
		type = Static::Type::ROOF;
		SetRect(32, 544, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "bush")
	{
		type = Static::Type::BUSH;
		SetRect(192, 513, 32, 28);
		SetPivot(16, 28);
	}

	if (name == "bush_2")
	{
		type = Static::Type::BUSH;
		SetRect(224, 515, 32, 28);
		SetPivot(16, 28);
	}

	if (name == "bush")
	{
		type = Static::Type::BUSH;
		SetRect(256, 513, 32, 28);
		SetPivot(16, 28);
	}

	// Cemetery sorting

	if (name == "statue_0")
	{
		type = Static::Type::STATUE;
		SetRect(486, 147, 20, 40);
		SetPivot(20, 40);
	}

	if (name == "tree_1")
	{
		type = Static::Type::TREE;
		SetRect(68, 193, 25, 31);
		SetPivot(13, 31);
	}

	if (name == "tree_2")
	{
		type = Static::Type::TREE;
		SetRect(194, 160, 62, 38);
		SetPivot(31, 38);
	}

	if (name == "light_1")
	{
		type = Static::Type::TREE;
		SetRect(115, 145, 11, 23);
		SetPivot(6, 23);
	}

	if (name == "stone_0")
	{
		type = Static::Type::STONE;
		SetRect(236, 21, 8, 25);
		SetPivot(4, 25);
	}

	if (name == "light_2")
	{
		type = Static::Type::LIGHT;
		SetRect(168, 130, 16, 25);
		SetPivot(8, 25);
	}

	if (name == "stone_2")
	{
		type = Static::Type::STONE;
		SetRect(194, 25, 28, 14);
		SetPivot(14, 14);
	}

	if (name == "stone_3")
	{
		type = Static::Type::STONE;
		SetRect(256, 143, 32, 17);
		SetPivot(16, 17);
	}

	if (name == "cemetery_door")
	{
		type = Static::Type::DOOR;
		SetRect(0, 229, 32, 27);
		SetPivot(16, 27);
	}

	if (name == "cemetery_door2")
	{
		type = Static::Type::DOOR;
		SetRect(32, 234, 32, 22);
		SetPivot(16, 22);
	}

	if (name == "cemetery_door3")
	{
		type = Static::Type::DOOR;
		SetRect(65, 229, 32, 27);
		SetPivot(16, 27);
	}

	// Medium city sorting

	if (name == "roof_red")
	{
		type = Static::Type::ROOF;
		SetRect(416, 236, 32, 20);
		SetPivot(16, 40);
	}

	if (name == "roof_red2")
	{
		type = Static::Type::ROOF;
		SetRect(448, 236, 32, 20);
		SetPivot(16, 40);
	}

	if (name == "roof_red3")
	{
		type = Static::Type::ROOF;
		SetRect(480, 236, 32, 20);
		SetPivot(16, 40);
	}

	if (name == "roof_red4")
	{
		type = Static::Type::ROOF;
		SetRect(416, 256, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_red5")
	{
		type = Static::Type::ROOF;
		SetRect(448, 256, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_red6")
	{
		type = Static::Type::ROOF;
		SetRect(480, 256, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "antenna")
	{
		type = Static::Type::ANTENNA;
		SetRect(195, 2, 27, 30);
		SetPivot(14, 90);
	}

	if (name == "antenna_2")
	{
		type = Static::Type::ANTENNA;
		SetRect(167, 14, 15, 18);
		SetPivot(8, 54);
	}

	if (name == "roof_green")
	{
		type = Static::Type::ROOF;
		SetRect(416, 12, 32, 20);
		SetPivot(16, 20);
	}

	if (name == "roof_green2")
	{
		type = Static::Type::ROOF;
		SetRect(448, 12, 32, 20);
		SetPivot(16, 20);
	}

	if (name == "roof_green3")
	{
		type = Static::Type::ROOF;
		SetRect(480, 12, 32, 20);
		SetPivot(16, 20);
	}

	if (name == "roof_green4")
	{
		type = Static::Type::ROOF;
		SetRect(448, 32, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "roof_green5")
	{
		type = Static::Type::ROOF;
		SetRect(480, 32, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "roof_green6")
	{
		type = Static::Type::ROOF;
		SetRect(416, 32, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "cup_city")
	{
		type = Static::Type::CUP;
		SetRect(73, 203, 14, 11);
		SetPivot(7, 22);
	}

	if (name == "machine_city")
	{
		type = Static::Type::MACHINE;
		SetRect(210, 150, 14, 38);
		SetPivot(7, 38);
	}

	if (name == "signal_city")
	{
		type = Static::Type::SIGNAL;
		SetRect(169, 32, 15, 40);
		SetPivot(8, 40);
	}

	if (name == "tree_city")
	{
		type = Static::Type::TREE;
		SetRect(100, 70, 25, 41);
		SetPivot(13, 41);
	}

	if (name == "roof_8")
	{
		type = Static::Type::ROOF;
		SetRect(416, 160, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_9")
	{
		type = Static::Type::ROOF;
		SetRect(480, 160, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_res")
	{
		type = Static::Type::ROOF;
		SetRect(353, 322, 31, 30);
		SetPivot(16, 30);
	}

	if (name == "roof_res2")
	{
		type = Static::Type::ROOF;
		SetRect(385, 322, 32, 30);
		SetPivot(16, 30);
	}

	if (name == "roof_res3")
	{
		type = Static::Type::ROOF;
		SetRect(417, 322, 32, 30);
		SetPivot(16, 30);
	}

	if (name == "roof_res4")
	{
		type = Static::Type::ROOF;
		SetRect(449, 322, 32, 30);
		SetPivot(16, 30);
	}

	if (name == "roof_res5")
	{
		type = Static::Type::ROOF;
		SetRect(481, 321, 32, 31);
		SetPivot(16, 30);
	}

	if (name == "roof_res6")
	{
		type = Static::Type::ROOF;
		SetRect(353, 352, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "roof_res7")
	{
		type = Static::Type::ROOF;
		SetRect(385, 352, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "roof_res8")
	{
		type = Static::Type::ROOF;
		SetRect(417, 352, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "roof_res9")
	{
		type = Static::Type::ROOF;
		SetRect(449, 352, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "roof_res10")
	{
		type = Static::Type::ROOF;
		SetRect(481, 352, 32, 32);
		SetPivot(16, 32);
	}

	// Restaurant sorting

	if (name == "door_res")
	{
		type = Static::Type::DOOR;
		SetRect(352, 192, 32, 32);
		SetPivot(16, 16);
	}

	if (name == "door_res2")
	{
		type = Static::Type::DOOR;
		SetRect(96, 122, 32, 6);
		SetPivot(32, 24);
	}

	if (name == "door_res3")
	{
		type = Static::Type::DOOR;
		SetRect(96, 192, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "door_res4")
	{
		type = Static::Type::DOOR;
		SetRect(128, 192, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "table_res")
	{
		type = Static::Type::TABLE;
		SetRect(0, 325, 32, 27);
		SetPivot(16, 27);
	}

	if (name == "table_res2")
	{
		type = Static::Type::TABLE;
		SetRect(32, 325, 32, 27);
		SetPivot(16, 27);
	}

	if (name == "table_res3")
	{
		type = Static::Type::TABLE;
		SetRect(64, 325, 32, 27);
		SetPivot(16, 27);
	}

	if (name == "table_res4")
	{
		type = Static::Type::TABLE;
		SetRect(0, 32, 32, 27);
		SetPivot(16, 27);
	}

	if (name == "table_res5")
	{
		type = Static::Type::TABLE;
		SetRect(0, 357, 32, 27);
		SetPivot(16, 27);
	}

	if (name == "table_res6")
	{
		type = Static::Type::TABLE;
		SetRect(32, 357, 32, 27);
		SetPivot(16, 27);
	}

	if (name == "res_table")
	{
		type = Static::Type::TABLE;
		SetRect(98, 34, 30, 29);
		SetPivot(128, 116);
	}

	if (name == "res_table2")
	{
		type = Static::Type::TABLE;
		SetRect(128, 37, 31, 26);
		SetPivot(124, 104);
	}

	if (name == "res_cupboard")
	{
		type = Static::Type::ROOF;
		SetRect(320, 272, 32, 16);
		SetPivot(16, 16);
	}

	if (name == "cash")
	{
		type = Static::Type::CASH;
		SetRect(256, 119, 32, 9);
		SetPivot(16, 9);
	}

	if (name == "cash_2")
	{
		type = Static::Type::CASH;
		SetRect(288, 110, 32, 18);
		SetPivot(16, 18);
	}

	if (name == "cash_3")
	{
		type = Static::Type::CASH;
		SetRect(256, 128, 32, 10);
		SetPivot(16, 10);
	}

	if (name == "cash_4")
	{
		type = Static::Type::CASH;
		SetRect(288, 128, 32, 10);
		SetPivot(16, 10);
	}

	if (name == "plate")
	{
		type = Static::Type::PLATE;
		SetRect(70, 358, 20, 14);
		SetPivot(80, 56);
	}

	if (name == "plate_2")
	{
		type = Static::Type::PLATE;
		SetRect(169, 363, 23, 9);
		SetPivot(92, 36);
	}

	if (name == "plate_3")
	{
		type = Static::Type::PLATE;
		SetRect(192, 363, 23, 9);
		SetPivot(92, 36);
	}

	if (name == "food")
	{
		type = Static::Type::FOOD;
		SetRect(168, 295, 18, 8);
		SetPivot(144, 64);
	}

	if (name == "food_2")
	{
		type = Static::Type::FOOD;
		SetRect(71, 0, 17, 19);
		SetPivot(136, 152);
	}

	if (name == "food_3")
	{
		type = Static::Type::FOOD;
		SetRect(236, 395, 20, 7);
		SetPivot(160, 56);
	}

	if (name == "food_4")
	{
		type = Static::Type::FOOD;
		SetRect(256, 395, 20, 7);
		SetPivot(160, 56);
	}

	if (name == "food_5")
	{
		type = Static::Type::FOOD;
		SetRect(171, 261, 10, 13);
		SetPivot(80, 104);
	}

	if (name == "floor")
	{
		type = Static::Type::FLOOR;
		SetRect(288, 80, 32, 16);
		SetPivot(16, 16);
	}

	// Big city sorting

	if (name == "lamp_city")
	{
		type = Static::Type::LAMP;
		SetRect(1130, 32, 12, 27);
		SetPivot(6, 27);
	}

	if (name == "lamp_city2")
	{
		type = Static::Type::LAMP;
		SetRect(1092, 64, 24, 32);
		SetPivot(12, 64);
	}

	if (name == "lamp_city3")
	{
		type = Static::Type::LAMP;
		SetRect(1098, 32, 12, 23);
		SetPivot(6, 23);
	}

	if (name == "lamp_city4")
	{
		type = Static::Type::LAMP;
		SetRect(1092, 0, 24, 32);
		SetPivot(12, 32);
	}

	if (name == "roof_city")
	{
		type = Static::Type::ROOF;
		SetRect(416, 192, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city2")
	{
		type = Static::Type::ROOF;
		SetRect(448, 192, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city3")
	{
		type = Static::Type::ROOF;
		SetRect(480, 192, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city4")
	{
		type = Static::Type::ROOF;
		SetRect(416, 256, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city5")
	{
		type = Static::Type::ROOF;
		SetRect(448, 256, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city6")
	{
		type = Static::Type::ROOF;
		SetRect(480, 256, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city7")
	{
		type = Static::Type::ROOF;
		SetRect(416, 224, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city8")
	{
		type = Static::Type::ROOF;
		SetRect(448, 224, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city9")
	{
		type = Static::Type::ROOF;
		SetRect(480, 224, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city10")
	{
		type = Static::Type::ROOF;
		SetRect(256, 128, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city11")
	{
		type = Static::Type::ROOF;
		SetRect(289, 128, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city12")
	{
		type = Static::Type::ROOF;
		SetRect(384, 128, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city13")
	{
		type = Static::Type::ROOF;
		SetRect(256, 160, 32, 32);
		SetPivot(16, 128);
	}

	if (name == "roof_city14")
	{
		type = Static::Type::ROOF;
		SetRect(384, 192, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city15")
	{
		type = Static::Type::ROOF;
		SetRect(320, 192, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city16")
	{
		type = Static::Type::ROOF;
		SetRect(384, 192, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "roof_city17")
	{
		type = Static::Type::ROOF;
		SetRect(512, 0, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "roof_city18")
	{
		type = Static::Type::ROOF;
		SetRect(544, 0, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "roof_city19")
	{
		type = Static::Type::ROOF;
		SetRect(576, 0, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "roof_city20")
	{
		type = Static::Type::ROOF;
		SetRect(512, 64, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "roof_city21")
	{
		type = Static::Type::ROOF;
		SetRect(544, 64, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "roof_city22")
	{
		type = Static::Type::ROOF;
		SetRect(576, 64, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "roof_city23")
	{
		type = Static::Type::ROOF;
		SetRect(384, 256, 32, 32);
		SetPivot(16, 64);
	}

	if (name == "furniture")
	{
		type = Static::Type::FURNITURE;
		SetRect(992, 113, 32, 15);
		SetPivot(16, 15);
	}

	if (name == "furniture_2")
	{
		type = Static::Type::FURNITURE;
		SetRect(992, 160, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "furniture_3")
	{
		type = Static::Type::FURNITURE;
		SetRect(992, 128, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "furniture_4")
	{
		type = Static::Type::FURNITURE;
		SetRect(708, 68, 28, 28);
		SetPivot(14, 28);
	}

	if (name == "furniture_5")
	{
		type = Static::Type::FURNITURE;
		SetRect(736, 69, 22, 28);
		SetPivot(11, 28);
	}

	if (name == "furniture_6")
	{
		type = Static::Type::FURNITURE;
		SetRect(712, 97, 24, 28);
		SetPivot(12, 28);
	}

	if (name == "furniture_7")
	{
		type = Static::Type::FURNITURE;
		SetRect(736, 97, 20, 31);
		SetPivot(10, 31);
	}

	if (name == "furniture_8")
	{
		type = Static::Type::FURNITURE;
		SetRect(960, 113, 32, 15);
		SetPivot(32, 60);
	}

	if (name == "furniture_9")
	{
		type = Static::Type::FURNITURE;
		SetRect(960, 160, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "furniture_10")
	{
		type = Static::Type::FURNITURE;
		SetRect(769, 265, 31, 19);
		SetPivot(31, 152);
	}

	if (name == "furniture_11")
	{
		type = Static::Type::FURNITURE;
		SetRect(960, 70, 32, 26);
		SetPivot(32, 104);
	}

	if (name == "furniture_12")
	{
		type = Static::Type::FURNITURE;
		SetRect(809, 256, 15, 30);
		SetPivot(15, 240);
	}

	if (name == "furniture_13")
	{
		type = Static::Type::FURNITURE;
		SetRect(963, 12, 27, 20);
		SetPivot(27, 80);
	}

	if (name == "furniture_14")
	{
		type = Static::Type::FURNITURE;
		SetRect(722, 4, 14, 28);
		SetPivot(14, 112);
	}

	if (name == "furniture_15")
	{
		type = Static::Type::FURNITURE;
		SetRect(724, 32, 12, 26);
		SetPivot(12, 104);
	}

	if (name == "furniture_16")
	{
		type = Static::Type::FURNITURE;
		SetRect(776, 393, 15, 19);
		SetPivot(15, 76);
	}

	if (name == "furniture_17")
	{
		type = Static::Type::FURNITURE;
		SetRect(864, 416, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "furniture_18")
	{
		type = Static::Type::FURNITURE;
		SetRect(896, 416, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "furniture_19")
	{
		type = Static::Type::FURNITURE;
		SetRect(864, 448, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "furniture_20")
	{
		type = Static::Type::FURNITURE;
		SetRect(896, 448, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "furniture_21")
	{
		type = Static::Type::FURNITURE;
		SetRect(898, 16, 28, 16);
		SetPivot(28, 64);
	}

	if (name == "furniture_22")
	{
		type = Static::Type::FURNITURE;
		SetRect(896, 32, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "furniture_23")
	{
		type = Static::Type::FURNITURE;
		SetRect(864, 288, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "furniture_24")
	{
		type = Static::Type::FURNITURE;
		SetRect(896, 288, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "furniture_25")
	{
		type = Static::Type::FURNITURE;
		SetRect(874, 480, 22, 15);
		SetPivot(22, 60);
	}

	if (name == "furniture_26")
	{
		type = Static::Type::FURNITURE;
		SetRect(896, 480, 21, 15);
		SetPivot(21, 60);
	}

	if (name == "furniture_27")
	{
		type = Static::Type::FURNITURE;
		SetRect(768, 416, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "furniture_28")
	{
		type = Static::Type::FURNITURE;
		SetRect(832, 416, 32, 32);
		SetPivot(32, 256);
	}

	if (name == "furniture_29")
	{
		type = Static::Type::FURNITURE;
		SetRect(968, 32, 17, 32);
		SetPivot(17, 128);
	}

	if (name == "furniture_30")
	{
		type = Static::Type::FURNITURE;
		SetRect(738, 149, 26, 11);
		SetPivot(13, 11);
	}

	if (name == "fence")
	{
		type = Static::Type::FENCE;
		SetRect(1120, 384, 32, 32);
		SetPivot(32, 128);
	}

	if (name == "fence_2")
	{
		type = Static::Type::FENCE;
		SetRect(1056, 366, 32, 18);
		SetPivot(16, 18);
	}

	if (name == "fence_3")
	{
		type = Static::Type::FENCE;
		SetRect(1088, 366, 32, 18);
		SetPivot(16, 18);
	}

	if (name == "fence_4")
	{
		type = Static::Type::FENCE;
		SetRect(1024, 366, 32, 18);
		SetPivot(16, 18);
	}

	if (name == "fence_5")
	{
		type = Static::Type::FENCE;
		SetRect(1216, 384, 32, 32);
		SetPivot(16, 32);
	}

	if (name == "fence_6")
	{
		type = Static::Type::FENCE;
		SetRect(1211, 398, 5, 18);
		SetPivot(5, 18);
	}

	if (name == "floating")
	{
		type = Static::Type::FLOATING;
		SetRect(1252, 17, 24, 23);
		SetPivot(12, 46);
	}

	if (name == "res_city_roof")
	{
		type = Static::Type::ROOF;
		SetRect(2100, 424, 160, 32);
		SetPivot(80, 32);
	}

	// Osaka sorting

	if (name == "roof_osaka")
	{
		type = Static::Type::ROOF;
		SetRect(32, 32, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka2")
	{
		type = Static::Type::ROOF;
		SetRect(64, 32, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka3")
	{
		type = Static::Type::ROOF;
		SetRect(32, 0, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka4")
	{
		type = Static::Type::ROOF;
		SetRect(384, 0, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka5")
	{
		type = Static::Type::ROOF;
		SetRect(416, 0, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka6")
	{
		type = Static::Type::ROOF;
		SetRect(448, 0, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka7")
	{
		type = Static::Type::ROOF;
		SetRect(544, 0, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka8")
	{
		type = Static::Type::ROOF;
		SetRect(576, 0, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka9")
	{
		type = Static::Type::ROOF;
		SetRect(608, 0, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka10")
	{
		type = Static::Type::ROOF;
		SetRect(544, 64, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka11")
	{
		type = Static::Type::ROOF;
		SetRect(577, 64, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka12")
	{
		type = Static::Type::ROOF;
		SetRect(608, 64, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka13")
	{
		type = Static::Type::ROOF;
		SetRect(128, 96, 32, 32);
		SetPivot(64, 64);
	}

	if (name == "roof_osaka14")
	{
		type = Static::Type::ROOF;
		SetRect(480, 0, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka15")
	{
		type = Static::Type::ROOF;
		SetRect(96, 32, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "roof_osaka16")
	{
		type = Static::Type::ROOF;
		SetRect(96, 64, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "osaka_furniture")
	{
		type = Static::Type::FURNITURE;
		SetRect(363, 13, 15, 13);
		SetPivot(15, 52);
	}

	if (name == "osaka_furniture2")
	{
		type = Static::Type::FURNITURE;
		SetRect(289, 17, 30, 15);
		SetPivot(30, 60);
	}

	if (name == "osaka_furniture3")
	{
		type = Static::Type::FURNITURE;
		SetRect(289, 32, 30, 21);
		SetPivot(30, 84);
	}

	if (name == "osaka_furniture4")
	{
		type = Static::Type::FURNITURE;
		SetRect(263, 204, 17, 20);
		SetPivot(9, 20);
	}

	if (name == "osaka_furniture5")
	{
		type = Static::Type::FURNITURE;
		SetRect(42, 232, 13, 24);
		SetPivot(13, 96);
	}

	if (name == "osaka_light")
	{
		type = Static::Type::LIGHT;
		SetRect(519, 229, 18, 81);
		SetPivot(9, 81);
	}

	if (name == "osaka_light2")
	{
		type = Static::Type::LIGHT;
		SetRect(106, 239, 20, 44);
		SetPivot(10, 44);
	}

	if (name == "osaka_light3")
	{
		type = Static::Type::LIGHT;
		SetRect(130, 239, 20, 44);
		SetPivot(10, 44);
	}

	if (name == "osaka_fence")
	{
		type = Static::Type::FENCE;
		SetRect(161, 271, 30, 17);
		SetPivot(15, 17);
	}

	if (name == "machine_osaka")
	{
		type = Static::Type::MACHINE;
		SetRect(491, 137, 21, 23);
		SetPivot(11, 23);
	}

	if (name == "machine_osaka2")
	{
		type = Static::Type::MACHINE;
		SetRect(491, 160, 21, 7);
		SetPivot(11, 7);
	}

	if (name == "machine_osaka3")
	{
		type = Static::Type::MACHINE;
		SetRect(517, 139, 21, 21);
		SetPivot(11, 21);
	}

	if (name == "machine_osaka4")
	{
		type = Static::Type::MACHINE;
		SetRect(517, 160, 21, 7);
		SetPivot(11, 7);
	}

	if (name == "road")
	{
		type = Static::Type::ROAD;
		SetRect(768, 384, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "road_2")
	{
		type = Static::Type::ROAD;
		SetRect(800, 384, 32, 32);
		SetPivot(32, 64);
	}

	if (name == "road_3")
	{
		type = Static::Type::ROAD;
		SetRect(832, 384, 32, 32);
		SetPivot(32, 64);
	}

	else {
		LOG("There isn't any type assigned to %s name entity", name.data());
	}

	size = iPoint(frame.w, frame.h);

	if (((Map*)entityManager->SearchEntity("map"))->data.type == MAPTYPE_ISOMETRIC)
	{
		position.x /= ((Map*)entityManager->SearchEntity("map"))->data.tileWidth * 0.5f;
		position.y /= ((Map*)entityManager->SearchEntity("map"))->data.tileHeight;

		iPoint pos = ((Map*)entityManager->SearchEntity("map"))->MapToWorld(position.x + 1, position.y + 1);
		position.Create(pos.x, pos.y);
	}

	SString mapTexturePath;
	mapTexturePath.Create(((Map*)entityManager->SearchEntity("map"))->data.properties.objectsPath.data());

	if (mapTexturePath == "Maps/Town/Tilemap.png")
	{
		data.tileset.texture = entityManager->townTexture;
	}
	else if (mapTexturePath == "Maps/MediumCity/mediumcity2.png")
	{
		data.tileset.texture = entityManager->mediumCityTexture;
	}
	else if (mapTexturePath == "Maps/Cemetery/GK_FG_B_2.png")
	{
		data.tileset.texture = entityManager->cemeteryTexture;
	}
	else if (mapTexturePath == "Maps/Restaurant/rest.png")
	{
		data.tileset.texture = entityManager->restaurantTexture;
	}
	else if (mapTexturePath == "Maps/bigCity/MainTileMap.png")
	{
		data.tileset.texture = entityManager->bigCityTexture;
	}
	else if (mapTexturePath == "Maps/Osaka/osaka_tile.png")
	{
		data.tileset.texture = entityManager->osakaTexture;
	}
}

Static::~Static()
{
}

bool Static::Draw(Render* render)
{
	bool ret = true;

	render->scale = 3;
	if (!render->DrawTexture(data.tileset.texture, position.x, position.y, &frame))
	{
		render->scale = 1;
		LOG("Could not draw the static entity texture...");
		ret = false;
	}
	render->scale = 1;
	
	return ret;
}

void Static::SetRect(int x, int y, int w, int h)
{
	frame.x = x;
	frame.y = y;
	frame.w = w;
	frame.h = h;
}
