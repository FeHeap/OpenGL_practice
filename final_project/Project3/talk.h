class talk {
public:
	const int talks_end[6] = {88, 118, 109, 84, 101, 66};

	const char talk_0[3][50] = {
		"hello ~ ~ ~ everybody!",
		"hello ~ ~ teamates!",
		"hello ~ investigators! hi~"
	};
	const int talk_clock_0[150] = {   0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  8,  8,  8, 10, 10, 10, 12, 12, 12, 22, 22, 22, 22, 22, 22,
									 23, 24, 25, 26, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 30, 30, 30, 32, 32, 32, 22 + 19, 22 + 19,
							         42, 43, 44, 45, 46, 47, 48, 48, 48, 50, 52, 55, 58, 61, 64, 66, 66, 66, 66, 66, 66, 66, 66, 66, 21 + 20 + 26};

	const char talk_1[3][60] = {
		"hello ~ how are you guys doing today?",
		"are you excited? oh",
		"I have to...oh there we go uh",
	};
	const int talk_clock_1[150] = {  0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8, 10, 12, 12, 14, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28, 30, 32, 35, 37, 37, 37,
									38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 37 + 19,
									57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 84, 37 + 19 + 29 };

	const char talk_2[2][60] = {
		"feels like a long time I...I was here. I was here earlier",
		"it's not my fault you weren't here hehe",
	};
	const int talk_clock_2[150] = {	0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 57, 57, 57, 57, 57,
								   58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 74, 74, 74, 74, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 92, 92, 92, 93, 94, 95, 96, 57+39};

	const char talk_3[2][50] = {
		"uh, I'm just closing out of stuff",
		"uh, I wanna I was open right",
	};
	const int talk_clock_3[150] = {	0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
								   34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 33+28};

	const char talk_4[3][60] = {
		"okay ~",
		"hopefully this works I tested earlier it should work yeah",
		"there we go"
	};
	const int talk_clock_4[150] = {	0,  1,  2,  3,  4,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
									7,  8,  9, 10, 11, 13, 15, 16, 17, 18, 19, 20, 21, 22, 23, 25, 27, 28, 29, 30, 31, 32, 34, 36, 38, 39, 41, 43, 45, 46, 47, 48, 49, 52, 54, 55, 57, 59, 60, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 6 + 57,
								   64, 66, 68, 69, 71, 73, 6 + 57 + 11};

	const char talk_5[2][50] = {
		"hi!",
		"hey guys I'm excited yeah",
	};
	const int talk_clock_5[150] = {	0,  1,  2,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
									4,  6,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 23, 23, 23, 23, 24, 25, 26, 27, 28, 3 + 25};
} Talk;
