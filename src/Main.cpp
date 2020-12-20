#include <Models/Data.h>
#include <Algorithms/FullEnumerationAlgorithm.h>

std::ofstream output;
int CheckedConditions;
int UncheckedConditions;

Data GetData() {
    auto data = Data();
    try {
        boost::property_tree::ptree pt;
        boost::property_tree::read_json("data.json", pt);
        data.MaxDistance = std::stoi(pt.get_child("MaxDistance").data());
        data.MaxBins = std::stoi(pt.get_child("MaxBins").data());
        std::vector<Entrance> entrances;
        for(auto &item : pt.get_child("Entrances")) {
            auto entrance = new Entrance(item.second.get_value<int>(), entrances.size());
            entrances.push_back(*entrance);
        }
        data.Entrances = std::move(entrances);
        std::vector<Bin> bins;
        for(auto &item : pt.get_child("Bins")) {
            auto bin = new Bin(item.second.get_value<int>(), bins.size());
            bins.push_back(*bin);
        }
        data.Bins = std::move(bins);
        std::vector<std::vector<double>> binEntranceDistances(data.Bins.size(), std::vector<double>(data.Entrances.size()));
        for(auto &route : pt.get_child("BinEntranceDistances")) {
            Bin bin;
            Entrance entrance;
            double distance = 0;
            for(auto &item : route.second) {
                if (item.first == "start_id") {
                    bin = data.GetBinFromId(item.second.get_value<int>());
                }
                if (item.first == "target_id") {
                    entrance = data.GetEntranceFromId(item.second.get_value<int>());
                }
                if (item.first == "distance") {
                    distance = item.second.get_value<double>();
                }
            }
            binEntranceDistances[bin.GetPosition()][entrance.GetPosition()] = distance;
        }
        data.BinEntranceDistances = std::move(binEntranceDistances);
        std::vector<std::vector<double>> binDistances(data.Bins.size(), std::vector<double>(data.Bins.size()));
        for(auto &route : pt.get_child("BinDistances")) {
            Bin bin1, bin2;
            double distance = 0;
            for(auto &item : route.second) {
                if (item.first == "start_id") {
                    bin1 = data.GetBinFromId(item.second.get_value<int>());
                }
                if (item.first == "target_id") {
                    bin2 = data.GetBinFromId(item.second.get_value<int>());
                }
                if (item.first == "distance") {
                    distance = item.second.get_value<double>();
                }
            }
            binDistances[bin1.GetPosition()][bin2.GetPosition()] = distance;
        }
        data.BinDistances = std::move(binDistances);

        return data;
    }
    catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;

        return data;
    }
}

void OutputResult(const std::shared_ptr<Model> model) {
    output << "Solution:";
    for(auto &item : model->GetRoute()) {
        output << item.GetId() << ",";
    }
    output << std::endl;
    output << "ObjFunctionValue:" << model->GetObjFunctionValue() << std::endl;
    output << "Statistics:" << std::endl;
    output << "CheckedConditions:" << CheckedConditions << std::endl;
    output << "UncheckedConditions:" << UncheckedConditions << std::endl;
}

int main() {
    output.open("output.txt");
    auto data = GetData();
    auto fullEnumerationAlgorithm = new FullEnumerationAlgorithm(data);
    auto model = fullEnumerationAlgorithm->GetMinModel();
    OutputResult(model);
    output.close();

    return EXIT_SUCCESS;
}