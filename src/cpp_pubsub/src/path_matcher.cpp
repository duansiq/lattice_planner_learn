#include "path_matcher.hpp"

PathPoint Path_Matcher::MatchToPath(const std::vector<PathPoint> &reference_line,
                                    const double x, const double y)
{
  // 计算规划起点和参考线上点距离最近的点的索引值
  auto func_distance_square = [](const PathPoint &point, const double x,
                                 const double y)
  {
    double dx = point.x - x;
    double dy = point.y - y;
    return dx * dx + dy * dy;
  };

  double distance_min = func_distance_square(reference_line.front(), x, y);
  std::size_t index_min = 0;

  for (std::size_t i = 1; i < reference_line.size(); ++i)
  {
    double distance_temp = func_distance_square(reference_line[i], x, y);
    if (distance_temp < distance_min)
    {
      distance_min = distance_temp;
      index_min = i;
    }
  }
  // 最近的点不是参考线的终点和起点对索引值分别+1 -1，得到两个索引值
  std::size_t index_start = (index_min == 0) ? index_min : index_min - 1;
  std::size_t index_end =
      (index_min + 1 == reference_line.size()) ? index_min : index_min + 1;
  // 会有这种情况出来吗？
  if (index_start == index_end)
  {
    return reference_line[index_start];
  }

  return FindProjectionPoint(reference_line[index_start],
                             reference_line[index_end], x, y);
}

std::pair<double, double> Path_Matcher::GetPathFrenetCoordinate(
    const std::vector<PathPoint> &reference_line, const double x,
    const double y)
{
  auto matched_path_point = MatchToPath(reference_line, x, y);
  double rtheta = matched_path_point.theta;
  double rx = matched_path_point.x;
  double ry = matched_path_point.y;
  double delta_x = x - rx;
  double delta_y = y - ry;
  double side = std::cos(rtheta) * delta_y - std::sin(rtheta) * delta_x;
  std::pair<double, double> relative_coordinate;
  relative_coordinate.first = matched_path_point.s;
  relative_coordinate.second =
      std::copysign(std::hypot(delta_x, delta_y), side);
  return relative_coordinate;
}

void Path_Matcher::MatchToPath(const std::vector<PathPoint> &reference_line, PathPoint &matched_ref_point,
                                    const double s)
{
  auto comp = [](const PathPoint &point, const double s)
  {
    return point.s < s;
  };
  auto it_lower = lower_bound(reference_line.begin(), reference_line.end(), s, comp);
  if (it_lower == reference_line.begin())
  {
    matched_ref_point = reference_line[0];
  }
  else if (it_lower == reference_line.end())
  {
        matched_ref_point =reference_line.back();
  }
  // interpolate between it_lower - 1 and it_lower
  // return interpolate(*(it_lower - 1), *it_lower, s);
  matched_ref_point=linerinterpolation.InterpolateUsingLinearApproximation(*(it_lower - 1), *it_lower, s);
  
  // int left = 0;
  // int right = reference_line.size() - 1;

  // while (left <= right)
  // {
  //   int mid = left + (right - left) / 2;
  //   if (s > reference_line[mid].s)
  //   {
  //     left = mid + 1;
  //   }
  //   else
  //   {
  //     right = mid - 1;
  //   }
  // }
  // cout << "left: " << left << " s: " << s << endl;
  // if (left > 0 && left < reference_line.size() - 1)
  // {
  //   PathPoint path_point;
  //   matched_ref_point=linerinterpolation.InterpolateUsingLinearApproximation(reference_line[left - 1], reference_line[left], s);
  //   cout << " x: " << matched_ref_point.x << " y: " << matched_ref_point.y << " s: " << matched_ref_point.s << endl;
  // }
  // else if (left == 0)
  // {
  //   // return linerinterpolation.InterpolateUsingLinearApproximation(reference_line[left], reference_line[left + 1], s);
  // }
  // else
  // {
  //   // return linerinterpolation.InterpolateUsingLinearApproximation(reference_line[left - 1], reference_line[left], s);
  // }
  // auto it_lower = &reference_line[left];
  // cout << " path matcher  s: " << s << endl;
  
}

PathPoint Path_Matcher::FindProjectionPoint(const PathPoint &p0,
                                            const PathPoint &p1, const double x,
                                            const double y)
{
  // 向量法计算delta_s
  double v0x = x - p0.x;
  double v0y = y - p0.y;

  double v1x = p1.x - p0.x;
  double v1y = p1.y - p0.y;

  double v1_norm = std::sqrt(v1x * v1x + v1y * v1y);
  double dot = v0x * v1x + v0y * v1y;

  double delta_s = dot / v1_norm;
  return linerinterpolation.InterpolateUsingLinearApproximation(p0, p1, p0.s + delta_s); // 线性插值计算匹配点
}