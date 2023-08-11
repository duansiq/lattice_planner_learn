#include "referenceline.hpp"
ReferenceLine::ReferenceLine()
{
    get_globalpath_from_txt();
    cal_referenceline();
}
ReferenceLine::~ReferenceLine() {}

// 计算全局路径点数
int cal_point_num(string &line_name);
// 车道中心线生成
vector<PathPoint> load_linecenter(string &line_name, int point_num, int parameter_num);

void ReferenceLine::cal_referenceline()
{
    reference_line = global_path;
}

void ReferenceLine::get_globalpath_from_txt()
{
    string globalpath = "/root/planner_ws/src/cpp_pubsub/txt/referenceline.txt";
    int num_line = cal_point_num(globalpath);
    global_path = load_linecenter(globalpath, num_line, 2);
}

// 计算全局路径点数
int cal_point_num(string &line_name)
{
    int row_index = 0; // 存储总的txt行数
    // char line[512];    // 每行最大字符数
    string temp;       // 存放每行的字符
    ifstream path_read;
    path_read.open(line_name, ios::in);

    if (!path_read.is_open())
    {
        cout << "Unable to open referenceline.txt";
        exit(1);
    }
    else
    {
        while (getline(path_read, temp))
        {
            row_index++;
        }
    }
    path_read.close();
    return row_index;
}

// 车道中心线生成
vector<PathPoint> load_linecenter(string &line_name, int point_num, int parameter_num)
{
    int path_row = point_num;
    int path_col = parameter_num;
    vector<vector<double>> global_path_array(point_num, vector<double>(parameter_num, 0));
    // double global_path_array[point_num][parameter_num] = {0}; // 临时存放初始化后的路径点
    PathPoint point_temp;                                // 临时容器变量
    vector<PathPoint> global_path;
    // 将txt文件内的全局路径点放入临时二维数组(x y)
    ifstream path_read;
    path_read.open(line_name, ios::in);

    if (!path_read.is_open())
    {
        cout << "Unable to open myfile";
        exit(1);
    }
    for (int i = 0; i < path_row; i++)
    {
        for (int j = 0; j < path_col; j++)
        {

            path_read >> global_path_array[i][j];
        }
    }
    path_read.close();

    // 计算路径点的theta kappa
    // 中点欧拉计算横摆角
    
    vector<double>  path_dx_pre_array(path_row);         // 前向dx
    vector<double>  path_dx_after_array(path_row);       // 后向dx
    vector<double>  path_dy_pre_array(path_row);         // 前向dy
    vector<double>  path_dy_after_array(path_row);       // 后向dy
    vector<double>  path_dx_final(path_row);             // 中点dx
    vector<double>  path_dy_final(path_row);             // 中点dy
    vector<double>  path_ds_final(path_row);             // 中点ds
    vector<double>  path_heading(path_row);              // 横摆角
    vector<double>  path_dheading_pre_array(path_row);   // 前向横摆角
    vector<double>  path_dheading_after_array(path_row); // 后向横摆角
    vector<double>  path_dheading_final(path_row);       // 中点横摆角
    vector<double>  path_kappa(path_row);                // 曲率kappa

    // 中点欧拉的dx dy计算
    for (int i = 0; i < path_row - 1; i++)
    {
        path_dx_pre_array[i + 1] = global_path_array[i + 1][0] - global_path_array[i][0];
        path_dx_after_array[i] = global_path_array[i + 1][0] - global_path_array[i][0];
        path_dy_pre_array[i + 1] = global_path_array[i + 1][1] - global_path_array[i][1];
        path_dy_after_array[i] = global_path_array[i + 1][1] - global_path_array[i][1];
    }

    path_dx_pre_array[0] = path_dx_pre_array[1];
    path_dx_after_array[path_row - 1] = path_dx_after_array[path_row - 2];
    path_dy_pre_array[0] = path_dy_pre_array[1];
    path_dy_after_array[path_row - 1] = path_dy_after_array[path_row - 2];

    for (int i = 0; i < path_row; i++)
    {
        path_dx_final[i] = 0.5 * (path_dx_pre_array[i] + path_dx_after_array[i]);
        path_dy_final[i] = 0.5 * (path_dy_pre_array[i] + path_dy_after_array[i]);
        path_ds_final[i] = sqrt(path_dx_final[i] * path_dx_final[i] + path_dy_final[i] * path_dy_final[i]);
        path_heading[i] = atan2(path_dy_final[i], path_dx_final[i]);
    }
    // 中点欧拉的横摆角计算
    for (int i = 0; i < path_row - 1; i++)
    {
        path_dheading_pre_array[i + 1] = path_heading[i + 1] - path_heading[i];
        path_dheading_after_array[i] = path_heading[i + 1] - path_heading[i];
    }

    path_dheading_pre_array[0] = path_dheading_pre_array[1];
    path_dheading_after_array[path_row - 1] = path_dheading_after_array[path_row - 2];
    for (int i = 0; i < path_row; i++)
    {
        path_dheading_final[i] = 0.5 * (path_dheading_pre_array[i] + path_dheading_after_array[i]);
        path_kappa[i] = sin(path_dheading_final[i]) / path_ds_final[i];
    }

    // 将全局路径路径点（x y theta kappa) 放进私有容器便于数据传导
    for (int i = 0; i < path_row; i++)
    {
        point_temp.x = global_path_array[i][0];
        point_temp.y = global_path_array[i][1];
        point_temp.theta = path_heading[i];
        point_temp.kappa = path_kappa[i];
        global_path.emplace_back(point_temp);
    }
    return global_path;
}
