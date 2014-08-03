/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#pragma once

#include "../itemset.hpp"
#include "../large.hpp"
#include "../ontologies/ontology.hpp"
#include "rule_node.hpp"

#include <vector>
#include <tuple>

#include <map>
#include <string>

using std::vector;
using std::tuple;

using std::map;
using std::string;

class Rules {
	private:
		vector<RuleNode *> rules;
		map<string, vector<RuleNode *>> rulesMap;
		
		vector<LargeItemSet *> larges;
		Ontology * ontologies;
		
		uint64_t amount_transactions;
		double confidence;
		std::map<string, uint64_t> itemset_1; //to calculate the amount of information
	public:
		Rules(uint64_t amount_transactions, double confidence, Ontology * ontologies, map<string, uint64_t> & itemset_1);
		Rules(string file);
		~Rules();
		
		void addLarge(LargeItemSet *);
		void computeRules();
		void print();
		
		uint64_t getFrequency(ItemSet * itemset);
		void calculateSemanticSimilarity();
		double informationMeasure(string identifier);
		
		vector<RuleNode *> & getRules();
		map<string, vector<RuleNode *>> & getRulesMap();
		
		void filterRules(); //TODO: implement Rules::filterRules()
};

bool rulesSort(const RuleNode * a, const RuleNode * b);
