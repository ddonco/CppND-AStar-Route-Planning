#include "route_planner.h"
#include <algorithm>
#include <iostream>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y) : m_Model(model)
{
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node)
{
    float dist = node->distance(*(this->end_node));
    return dist;
}

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node)
{
    current_node->FindNeighbors();
    for (RouteModel::Node *neighbor_node : current_node->neighbors)
    {
        neighbor_node->parent = current_node;
        neighbor_node->g_value = current_node->g_value + current_node->distance(*neighbor_node);
        neighbor_node->h_value = CalculateHValue(neighbor_node);
        neighbor_node->visited = true;
        this->open_list.emplace_back(neighbor_node);
    }
}

RouteModel::Node *RoutePlanner::NextNode()
{
    std::sort(this->open_list.begin(), this->open_list.end(),
              [](auto const &a, auto const &b) {
                  return (a->h_value + a->g_value) > (b->h_value + b->g_value);
              });
    RouteModel::Node *lowest_f = this->open_list.back();
    this->open_list.pop_back();
    return lowest_f;
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node)
{
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    while (current_node->parent != nullptr)
    {
        path_found.emplace_back(*current_node);
        distance += current_node->distance(*(current_node->parent));
        current_node = current_node->parent;
    }

    path_found.emplace_back(*current_node);
    reverse(path_found.begin(), path_found.end());
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;
}

void RoutePlanner::AStarSearch()
{
    RouteModel::Node *current_node = nullptr;

    this->start_node->visited = true;
    this->open_list.emplace_back(this->start_node);

    while (this->open_list.size() > 0)
    {
        current_node = NextNode();
        if (current_node->distance(*(this->end_node)) == 0)
        {
            m_Model.path = ConstructFinalPath(current_node);
            return;
        }
        AddNeighbors(current_node);
    }
}
