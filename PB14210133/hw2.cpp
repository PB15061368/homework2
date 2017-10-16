#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

#define MY_OK 1
#define MY_FAIL -1
#define CIRCLE_RADIUS_DIFF 5
#define GRAD_MAG_TH 100
#define EDGE_NUM_TH 300
#define MAX_CIRCLES 1000

float my_neg_sin[360] = { -0.000000,-0.017452,-0.034899,-0.052336,-0.069756,-0.087156,-0.104528,-0.121869,-0.139173,-0.156434,-0.173648,-0.190809,-0.207912,-0.224951,-0.241922,-0.258819,-0.275637,-0.292372,-0.309017,-0.325568,-0.342020,-0.358368,-0.374607,-0.390731,-0.406737,-0.422618,-0.438371,-0.453991,-0.469472,-0.484810,-0.500000,-0.515038,-0.529919,-0.544639,-0.559193,-0.573576,-0.587785,-0.601815,-0.615662,-0.629320,-0.642788,-0.656059,-0.669131,-0.681998,-0.694658,-0.707107,-0.719340,-0.731354,-0.743145,-0.754710,-0.766044,-0.777146,-0.788011,-0.798635,-0.809017,-0.819152,-0.829038,-0.838671,-0.848048,-0.857167,-0.866025,-0.874620,-0.882948,-0.891007,-0.898794,-0.906308,-0.913545,-0.920505,-0.927184,-0.933580,-0.939693,-0.945519,-0.951057,-0.956305,-0.961262,-0.965926,-0.970296,-0.974370,-0.978148,-0.981627,-0.984808,-0.987688,-0.990268,-0.992546,-0.994522,-0.996195,-0.997564,-0.998630,-0.999391,-0.999848,-1.000000,-0.999848,-0.999391,-0.998630,-0.997564,-0.996195,-0.994522,-0.992546,-0.990268,-0.987688,-0.984808,-0.981627,-0.978148,-0.974370,-0.970296,-0.965926,-0.961262,-0.956305,-0.951056,-0.945519,-0.939693,-0.933580,-0.927184,-0.920505,-0.913545,-0.906308,-0.898794,-0.891007,-0.882948,-0.874620,-0.866025,-0.857167,-0.848048,-0.838671,-0.829038,-0.819152,-0.809017,-0.798636,-0.788011,-0.777146,-0.766044,-0.754710,-0.743145,-0.731354,-0.719340,-0.707107,-0.694658,-0.681998,-0.669131,-0.656059,-0.642788,-0.629320,-0.615662,-0.601815,-0.587785,-0.573576,-0.559193,-0.544639,-0.529919,-0.515038,-0.500000,-0.484810,-0.469472,-0.453990,-0.438371,-0.422618,-0.406737,-0.390731,-0.374607,-0.358368,-0.342020,-0.325568,-0.309017,-0.292372,-0.275637,-0.258819,-0.241922,-0.224951,-0.207912,-0.190809,-0.173648,-0.156435,-0.139173,-0.121869,-0.104529,-0.087156,-0.069757,-0.052336,-0.034900,-0.017453,-0.000000, 0.017452, 0.034900, 0.052336, 0.069757, 0.087156, 0.104528, 0.121869, 0.139173, 0.156434, 0.173648, 0.190809, 0.207912, 0.224951, 0.241922, 0.258819, 0.275637, 0.292372, 0.309017, 0.325568, 0.342020, 0.358368, 0.374606, 0.390731, 0.406737, 0.422618, 0.438371, 0.453990, 0.469472, 0.484810, 0.500000, 0.515038, 0.529919, 0.544639, 0.559193, 0.573576, 0.587785, 0.601815, 0.615661, 0.629320, 0.642788, 0.656059, 0.669131, 0.681998, 0.694658, 0.707107, 0.719340, 0.731354, 0.743145, 0.754710, 0.766044, 0.777146, 0.788011, 0.798635, 0.809017, 0.819152, 0.829037, 0.838671, 0.848048, 0.857167, 0.866025, 0.874620, 0.882948, 0.891007, 0.898794, 0.906308, 0.913545, 0.920505, 0.927184, 0.933580, 0.939693, 0.945519, 0.951056, 0.956305, 0.961262, 0.965926, 0.970296, 0.974370, 0.978148, 0.981627, 0.984808, 0.987688, 0.990268, 0.992546, 0.994522, 0.996195, 0.997564, 0.998630, 0.999391, 0.999848, 1.000000, 0.999848, 0.999391, 0.998630, 0.997564, 0.996195, 0.994522, 0.992546, 0.990268, 0.987688, 0.984808, 0.981627, 0.978148, 0.974370, 0.970296, 0.965926, 0.961262, 0.956305, 0.951057, 0.945519, 0.939693, 0.933580, 0.927184, 0.920505, 0.913545, 0.906308, 0.898794, 0.891007, 0.882948, 0.874620, 0.866026, 0.857167, 0.848048, 0.838671, 0.829038, 0.819152, 0.809017, 0.798636, 0.788011, 0.777146, 0.766045, 0.754710, 0.743145, 0.731354, 0.719340, 0.707107, 0.694659, 0.681998, 0.669131, 0.656059, 0.642788, 0.629321, 0.615661, 0.601815, 0.587785, 0.573576, 0.559193, 0.544639, 0.529919, 0.515038, 0.500000, 0.484810, 0.469472, 0.453991, 0.438371, 0.422618, 0.406737, 0.390731, 0.374607, 0.358368, 0.342020, 0.325568, 0.309017, 0.292372, 0.275637, 0.258819, 0.241922, 0.224951, 0.207912, 0.190809, 0.173648, 0.156434, 0.139173, 0.121870, 0.104528, 0.087156, 0.069757, 0.052336, 0.034900, 0.017452 };
float my_cos[360] = { 1.000000, 0.999848, 0.999391, 0.998630, 0.997564, 0.996195, 0.994522, 0.992546, 0.990268, 0.987688, 0.984808, 0.981627, 0.978148, 0.974370, 0.970296, 0.965926, 0.961262, 0.956305, 0.951057, 0.945519, 0.939693, 0.933580, 0.927184, 0.920505, 0.913545, 0.906308, 0.898794, 0.891007, 0.882948, 0.874620, 0.866025, 0.857167, 0.848048, 0.838671, 0.829038, 0.819152, 0.809017, 0.798635, 0.788011, 0.777146, 0.766044, 0.754710, 0.743145, 0.731354, 0.719340, 0.707107, 0.694658, 0.681998, 0.669131, 0.656059, 0.642788, 0.629320, 0.615661, 0.601815, 0.587785, 0.573576, 0.559193, 0.544639, 0.529919, 0.515038, 0.500000, 0.484810, 0.469472, 0.453991, 0.438371, 0.422618, 0.406737, 0.390731, 0.374607, 0.358368, 0.342020, 0.325568, 0.309017, 0.292372, 0.275637, 0.258819, 0.241922, 0.224951, 0.207912, 0.190809, 0.173648, 0.156435, 0.139173, 0.121869, 0.104528, 0.087156, 0.069756, 0.052336, 0.034900, 0.017452, 0.000000,-0.017452,-0.034900,-0.052336,-0.069756,-0.087156,-0.104529,-0.121869,-0.139173,-0.156434,-0.173648,-0.190809,-0.207912,-0.224951,-0.241922,-0.258819,-0.275637,-0.292372,-0.309017,-0.325568,-0.342020,-0.358368,-0.374607,-0.390731,-0.406737,-0.422618,-0.438371,-0.453990,-0.469472,-0.484810,-0.500000,-0.515038,-0.529919,-0.544639,-0.559193,-0.573576,-0.587785,-0.601815,-0.615662,-0.629320,-0.642788,-0.656059,-0.669131,-0.681998,-0.694658,-0.707107,-0.719340,-0.731354,-0.743145,-0.754710,-0.766044,-0.777146,-0.788011,-0.798635,-0.809017,-0.819152,-0.829037,-0.838671,-0.848048,-0.857167,-0.866025,-0.874620,-0.882948,-0.891007,-0.898794,-0.906308,-0.913545,-0.920505,-0.927184,-0.933580,-0.939693,-0.945519,-0.951056,-0.956305,-0.961262,-0.965926,-0.970296,-0.974370,-0.978148,-0.981627,-0.984808,-0.987688,-0.990268,-0.992546,-0.994522,-0.996195,-0.997564,-0.998630,-0.999391,-0.999848,-1.000000,-0.999848,-0.999391,-0.998630,-0.997564,-0.996195,-0.994522,-0.992546,-0.990268,-0.987688,-0.984808,-0.981627,-0.978148,-0.974370,-0.970296,-0.965926,-0.961262,-0.956305,-0.951057,-0.945519,-0.939693,-0.933580,-0.927184,-0.920505,-0.913545,-0.906308,-0.898794,-0.891007,-0.882948,-0.874620,-0.866026,-0.857167,-0.848048,-0.838671,-0.829038,-0.819152,-0.809017,-0.798636,-0.788011,-0.777146,-0.766044,-0.754710,-0.743145,-0.731354,-0.719340,-0.707107,-0.694659,-0.681998,-0.669131,-0.656059,-0.642788,-0.629321,-0.615661,-0.601815,-0.587785,-0.573576,-0.559193,-0.544639,-0.529919,-0.515038,-0.500000,-0.484810,-0.469472,-0.453990,-0.438371,-0.422618,-0.406737,-0.390731,-0.374607,-0.358368,-0.342020,-0.325568,-0.309017,-0.292372,-0.275637,-0.258819,-0.241922,-0.224951,-0.207912,-0.190809,-0.173648,-0.156434,-0.139173,-0.121870,-0.104528,-0.087156,-0.069757,-0.052336,-0.034900,-0.017452,-0.000000, 0.017452, 0.034900, 0.052336, 0.069757, 0.087156, 0.104528, 0.121869, 0.139173, 0.156434, 0.173648, 0.190809, 0.207912, 0.224951, 0.241922, 0.258819, 0.275637, 0.292371, 0.309017, 0.325568, 0.342020, 0.358368, 0.374606, 0.390731, 0.406737, 0.422618, 0.438371, 0.453990, 0.469472, 0.484810, 0.500000, 0.515038, 0.529919, 0.544639, 0.559193, 0.573576, 0.587785, 0.601815, 0.615661, 0.629320, 0.642788, 0.656059, 0.669131, 0.681998, 0.694658, 0.707107, 0.719340, 0.731354, 0.743145, 0.754710, 0.766044, 0.777146, 0.788011, 0.798635, 0.809017, 0.819152, 0.829037, 0.838671, 0.848048, 0.857167, 0.866025, 0.874620, 0.882947, 0.891007, 0.898794, 0.906308, 0.913545, 0.920505, 0.927184, 0.933580, 0.939693, 0.945519, 0.951056, 0.956305, 0.961262, 0.965926, 0.970296, 0.974370, 0.978148, 0.981627, 0.984808, 0.987688, 0.990268, 0.992546, 0.994522, 0.996195, 0.997564, 0.998630, 0.999391, 0.999848 };

int ustc_Find_Circles_By_Difference(Mat colorImg, int min_radius, int max_radius, int min_center_dist, int min_radius_dist, int max_circle_diff, int* x, int* y, int* radius, int* circle_cnt, int max_circle) {
	if (NULL == colorImg.data || CV_8UC3 != colorImg.type()) {
		cout << "Input image is invalid." << endl;
		return MY_FAIL;
	}
	if (NULL == x || NULL == y || NULL == radius || NULL == circle_cnt) {
		cout << "Input pointer is null." << endl;
		return MY_FAIL;
	}
	if (min_radius < CIRCLE_RADIUS_DIFF || max_radius < min_radius || min_center_dist < 0 || min_radius_dist < 0 || max_circle_diff < 0 || max_circle < 0) {
		cout << "Input parameter is invalid." << endl;
		return MY_FAIL;
	}
	*circle_cnt = 0;
	int height = colorImg.rows, width = colorImg.cols;

	Mat grad_x, grad_y, mag;
	Sobel(colorImg, grad_x, CV_32F, 1, 0, 3, 1.0, 0.0, BORDER_DEFAULT);
	Sobel(colorImg, grad_y, CV_32F, 0, 1, 3, 1.0, 0.0, BORDER_DEFAULT);
	magnitude(grad_x, grad_y, mag);
	Mat mag_binary(height, width, CV_8UC3, Scalar(0, 0, 0));
	for (int height_y = 0; height_y < height; height_y++) {
		float *mag_line = mag.ptr<float>(height_y);
		uint8_t *binary_line = mag_binary.ptr<uint8_t>(height_y);
		for (int width_x = 0; width_x < width * 3; width_x++) {
			if (mag_line[width_x] > GRAD_MAG_TH) {
				binary_line[width_x] = 1;
			}
		}
	}

	Mat mean_pixel_diff(height, width, CV_32SC3, Scalar(0, 0, 0));
	Mat edge_num(height, width, CV_32SC3, Scalar(0, 0, 0));

	for (int circle_r = min_radius; circle_r <= max_radius; circle_r++) {
		mean_pixel_diff.setTo(Scalar(0, 0, 0));
		edge_num.setTo(Scalar(0, 0, 0));
		for (int theta = 0; theta < 360; theta++) {
			int start_x, end_x, start_y, end_y;
			int offset_x_in = (int)((circle_r - CIRCLE_RADIUS_DIFF)*my_cos[theta]) * 3;
			int offset_y_in = (circle_r - CIRCLE_RADIUS_DIFF)*my_neg_sin[theta];
			int offset_x_r = (int)(circle_r*my_cos[theta]) * 3;
			int offset_y_r = circle_r*my_neg_sin[theta];
			int offset_x_out = (int)((circle_r + CIRCLE_RADIUS_DIFF)*my_cos[theta]) * 3;
			int offset_y_out = (circle_r + CIRCLE_RADIUS_DIFF)*my_neg_sin[theta];
			start_y = circle_r + CIRCLE_RADIUS_DIFF;
			start_x = start_y * 3;
			end_y = height - start_y;
			end_x = width * 3 - start_x;
			for (int height_y = start_y; height_y < end_y; height_y++) {
				int32_t *mean_pixel_diff_line = mean_pixel_diff.ptr<int32_t>(height_y);
				int32_t *edge_num_line = edge_num.ptr<int32_t>(height_y);
				uint8_t *colorImg_in_line = colorImg.ptr<uint8_t>(height_y + offset_y_in);
				uint8_t *colorImg_out_line = colorImg.ptr<uint8_t>(height_y + offset_y_out);
				uint8_t *mag_binary_line = mag_binary.ptr<uint8_t>(height_y + offset_y_r);
				for (int width_x = start_x; width_x < end_x; width_x++) {
					mean_pixel_diff_line[width_x] += colorImg_in_line[width_x + offset_x_in] - colorImg_out_line[width_x + offset_x_out];
					edge_num_line[width_x] += mag_binary_line[width_x + offset_x_r];
				}
			}
		}
		for (int height_y = 0; height_y < height; height_y++) {
			int32_t *mean_pixel_diff_line = mean_pixel_diff.ptr<int32_t>(height_y);
			int32_t *edge_num_line = edge_num.ptr<int32_t>(height_y);
			for (int width_x = 0; width_x < width; width_x++) {
				int width_x_3 = 3 * width_x;
				if (edge_num_line[width_x_3] >= EDGE_NUM_TH || edge_num_line[width_x_3 + 1] >= EDGE_NUM_TH || edge_num_line[width_x_3 + 2] >= EDGE_NUM_TH) {
					int b_diff = mean_pixel_diff_line[width_x_3] / 360;
					int g_diff = mean_pixel_diff_line[width_x_3 + 1] / 360;
					int r_diff = mean_pixel_diff_line[width_x_3 + 2] / 360;
					if (b_diff < 0) {
						b_diff = -b_diff;
					}
					if (g_diff < 0) {
						g_diff = -g_diff;
					}
					if (r_diff < 0) {
						r_diff = -r_diff;
					}
					int circle_num = *circle_cnt;
					int diff[MAX_CIRCLES];
					if ((b_diff >= max_circle_diff || g_diff >= max_circle_diff || r_diff >= max_circle_diff) && circle_num < MAX_CIRCLES) {
						bool tag = true;
						int i;
						for (i = 0; i < circle_num; i++) {
							int x_diff = x[i] - width_x;
							int y_diff = y[i] - height_y;
							int radius_diff = radius[i] - circle_r;
							if (x_diff < 0) {
								x_diff = -x_diff;
							}
							if (y_diff < 0) {
								y_diff = -y_diff;
							}
							if (radius_diff < 0) {
								radius_diff = -radius_diff;
							}
							if (x_diff < min_center_dist && y_diff < min_center_dist && radius_diff < min_radius_dist) {
								if (b_diff + g_diff + r_diff <= diff[i]) {
									tag = false;
								}
								break;
							}
						}
						if (tag) {
							x[i] = width_x;
							y[i] = height_y;
							radius[i] = circle_r;
							diff[i] = b_diff + g_diff + r_diff;
							if (i == circle_num&&circle_num < max_circle) {
								(*circle_cnt)++;
							}
						}
					}
				}
			}
		}
	}
	return MY_OK;
}