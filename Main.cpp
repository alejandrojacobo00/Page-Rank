//Written by Alejandro Jacobo
#include <iostream>
#include <iomanip>
#include <map>
#include <utility>
#include <vector>
using namespace std;


class AdjacencyList {
	private:
		//This will hold the name of a node and a vector of all its OUTBOUND connections
		map<string, vector<string>> AdjList;

		//This will hold a node and a vector of all its INBOUND connections. This will be useful for calculating pagerank since we arent using a matrix
		map<string, vector<string>> ReverseAdjList;


		//This is the "r" vector they use in the example to multiply against the matrix and then also to print out the page rank after "x" power iterations
		map<string, double> Rank;
		map<string, double> RankTemp;

	public:

		void insert(string from, string to) {
			//Step 1. Check if the "from" node exists. If not, add it. Check if "to" node exists, if not add it
			//Create the inverseMatrix in the reverse fashion

			//AdjacenyList
			map<string, vector<string>>::iterator itr;
			itr = AdjList.find(from);

			if (itr == AdjList.end()) {
				vector<string> empty;
				AdjList.emplace(from, empty);				
			}

			itr = AdjList.find(to);
			if (itr == AdjList.end()) {
				vector<string> empty;
				AdjList.emplace(to, empty);
			}

			//ReverseAdjList
			itr = ReverseAdjList.find(to);

			if (itr == ReverseAdjList.end()) {
				vector<string> empty;
				ReverseAdjList.emplace(to, empty);
			}

			itr = ReverseAdjList.find(from);
			if (itr == ReverseAdjList.end()) {
				vector<string> empty;
				ReverseAdjList.emplace(from, empty);
			}



			//Step 2: Add the "to" connection to AdjacenyList. Add the "from" connection to ReverseAdjList
			itr = AdjList.find(from);
			itr->second.push_back(to);	

			itr = ReverseAdjList.find(to);
			itr->second.push_back(from);
		}

		void CalcPageRank(int PowerItr){			
			//Step 1. Initialize the map "r0" of size "Nodes" to be equal to (1/Nodes) so that we can multiply it agaisnt out adjaceny list if PowerIteration is > 1	
			//We emblace the name asell so we can easily do the dot product
			map<string, vector<string>>::iterator Inbound;
			map<string, vector<string>>::iterator Outbound;
			map<string, double>::iterator Search;

			double initial = (1.0 / double(AdjList.size()));
			for (Outbound = AdjList.begin(); Outbound != AdjList.end(); Outbound++) {
				Rank.emplace(Outbound->first, initial);
			}


			//We only return the initialized vector
			if (PowerItr == 1) {				
				return;
			}

			//Step 2: If PowerItr is greater than 1, then we have to do a dot product between our adjacent list and the RankVector. We then update the RankVector with the 
			//Output of this formula and repeat until we finish the loop. According to the document, we must compute the dot product. To simulate this, we need to create 
			//a vector which is the sum of the ranks of a nodes inbound connections. We only need to do this 
			//once, as the values will stay constant unless more nodes are added.
			for (int x = 1; x < PowerItr; x++) {
				for (Inbound = ReverseAdjList.begin(); Inbound != ReverseAdjList.end(); Inbound++) {
					double DotProductOutcome = 0.0;
					for (int i = 0; i < Inbound->second.size(); i++) {
						string InboundName = Inbound->second.at(i);
						double Value = getOutboundValue(InboundName);
						Search = Rank.find(InboundName);

						double tempDot = Value * Search->second;
						DotProductOutcome += tempDot;
					}					
					RankTemp[Inbound->first] = DotProductOutcome;
				}		
				Rank = RankTemp;
			}		
		}

		double getOutboundValue(string name) {
			int OutboundLinks = AdjList.find(name)->second.size();
			return (1.0 / double(OutboundLinks));
		}

		void TestPrintMaps(){
			map<string, vector<string>>::iterator itr = AdjList.begin();

			cout << endl << endl;
			cout << "Testing AdjacenyList" << endl;
			cout << "--------------" << endl << endl;
			for (int i = 0; i < AdjList.size(); i++) {
				cout << "Name of node: " << itr->first << endl;
				for (int j = 0; j < itr->second.size(); j++) {
					cout << "Has a link to: " << itr->second.at(j) << endl;
				}
				cout << "Total # of OUTBOUND links: " << itr->second.size() << endl;
				cout << endl << endl;
				itr++;
			}

			cout << endl << endl;
			itr = ReverseAdjList.begin();
			cout << "Testing ReverseAdjacenyList" << endl;
			cout << "--------------" << endl << endl;
			for (int i = 0; i < ReverseAdjList.size(); i++) {
				cout << "Name of node: " << itr->first << endl;
				for (int j = 0; j < itr->second.size(); j++) {
					cout << "Has an inbound link from: " << itr->second.at(j) << endl;
				}
				cout << "Total # of INBOUND links: " << itr->second.size() << endl;
				cout << endl << endl;
				itr++;
			}


		}

		void PrintRanks() {
			map<string, double>::iterator itr = Rank.begin();
			cout << std::fixed;
			std::cout << std::setprecision(2);
			for (itr = Rank.begin(); itr != Rank.end(); itr++) {
				cout << itr->first << " "  << itr->second << endl;
			}
		}
};


int main() {
	AdjacencyList test;
	int no_of_lines;
	int power_iterations;
	string from;
	string to;

	cin >> no_of_lines;
	std::cin >> power_iterations; 
	
	for (int i = 0; i < no_of_lines; i++) {
		cin >> from;
		cin >> to;
		test.insert(from, to);
	}
	
	test.CalcPageRank(power_iterations);
	cout << endl << endl;
	test.PrintRanks();
}
