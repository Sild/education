#include <vector>
#include <iostream>

using namespace std;

class Solution {
public:

    void Sum(vector<int>& candidates, int target, vector<vector<int>>& sRes, vector<int>& sCurSolution, int i)
    {
        std::cout << "sum: i=" << i << ", curSol=";
        for (auto& e: sCurSolution)
            std::cout << e << ",";
        std::cout << std::endl;
        if(target == 0)
        {
            std::cout << "match!" << std::endl;
            // if we get exact answer
            sRes.push_back(sCurSolution);
            return;
        }
        
        while(i < candidates.size() && target - candidates[i] >= 0)
        {
            // Till every element in the array starting
            // from i which can contribute to the target
            sCurSolution.push_back(candidates[i]);// add them to vector
            
            // recur for next numbers
            Sum(candidates, target - candidates[i], sRes, sCurSolution, i);
            ++i;
            
            // Remove number from vector (backtracking)
            std::cout << "backtrack" << std::endl;
            sCurSolution.pop_back();
        }
}
    
     
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(),candidates.end()); // sort candidates array
        
        vector<int> sCurrSolution;
        vector<vector<int> > sRes;
        std::cout << "input: ";
        for(auto& e: candidates)
            std::cout << e << ",";
        std::cout << std::endl;

        Sum(candidates,target,sRes,sCurrSolution,0);
        
        return sRes;
    }
};

int main() {
    Solution sol;
    std::vector<int> src = {2,3,6,7};
    auto res = sol.combinationSum(src, 7);
}