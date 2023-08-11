#include "obstacle.hpp"

// 计算txt总行数
int cal_txt_row()
{
    int row_index = 0; // 存储总的txt行数
    // char line[512];    // 每行最大字符数
    string temp; // 存放每行的字符
    ifstream obs_read;
    obs_read.open("/root/planner_ws/src/cpp_pubsub/txt/staticobs.txt", ios::in);

    if (!obs_read.is_open())
    {
        cout << "Unable to open staticobs.txt";
        exit(1);
    }
    else
    {
        while (getline(obs_read, temp))
        {
            row_index++;
        }
    }
    obs_read.close();
    return row_index;
}

void Obstacle::get_obs_from_txt()
{
    int txt_num = cal_txt_row();
    if (txt_num != 0)
    {
        vector<vector<double>> obs(txt_num, vector<double>(4, 0));

        ifstream obs_read;
        obs_read.open("/root/planner_ws/src/cpp_pubsub/txt/staticobs.txt");

        if (!obs_read.is_open())
        {
            cout << "Unable to open staticobs.txt";
            exit(1);
        }

        for (int i = 0; i < txt_num; i++)
        {
            obs_read >> obs[i][0]; //x
            obs_read >> obs[i][1]; //y
            obs_read >> obs[i][2]; //heading
            obs_read >> obs[i][3]; //v
        }
        obs_read.close();
        obs_gcs.clear();
        for (auto obs_ : obs)
        {
            obs_gcs.emplace_back(obs_);
        }
    }
}


