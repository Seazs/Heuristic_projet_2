#include "PFSP.hpp"
#include "utils.hpp"
#include "ils.hpp"
#include "genetic.hpp"




int main(int argc, char* argv[]) {

    std::string dataFileName, sls_type;
    handleProgramParameters(argc, argv, dataFileName, sls_type);
    PFSP pfsp(dataFileName.c_str());

    std::vector<int> algo_result_jobs_order;
    if (sls_type == "--ils") {
        algo_result_jobs_order = ils(pfsp);

    } else if (sls_type == "--genetic") {
        algo_result_jobs_order = genetic(pfsp);
    } else {
        std::cerr << "Invalid SLS type. Use --ils or --genetic." << std::endl;
        return 1;
    }

    int TotalCompletionTime = pfsp.getTotalCompletionTime(algo_result_jobs_order);

    

    return 0;
}