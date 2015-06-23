#ifndef FISCORE_H
#define FISCORE_H

#include <algorithm> //does this break things on non-VS?
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <QString>
#include "habitat_misc.h"

namespace HabitatModel {

enum FISOperatorAnd {FISOpAnd_Min, FISOpAnd_Product, FISOpAnd_None};
enum FISOperatorOr {FISOpOr_Max, FISOpOr_Probor, FISOpOr_None};
enum FISOperator {FISOp_And, FISOp_Or, FISOp_None};
enum FISImplicator {FISImp_Min, FISImp_Product, FISImp_None};
enum FISAggregator {FISAgg_Max, FISAgg_Probor, FISAgg_Sum, FISAgg_None};
enum FISDefuzzifier {FISDefuzz_Centroid, FISDefuzz_Bisect, FISDefuzz_MidMax, FISDefuzz_LargeMax,
                     FISDefuzz_SmallMax, FISDefuzz_None};


typedef std::vector<double> FISCoords;
typedef std::vector<double> FISValues;

bool FISIntersectLines(double ax1, double ay1, double ax2, double ay2, double bx1, double by1,
                                             double bx2, double by2, double* x, double* y);
double FISOpMax(double n1, double n2);
double FISOpMin(double n1, double n2);
double FISOpProbor(double n1, double n2);
double FISOpProduct(double n1, double n2);


/**
 * @brief
 *
 */
class FISException : public std::runtime_error {
public:
    FISException(QString const& msg) : std::runtime_error(msg.toStdString()) {}
};


/**
 * @brief
 *
 */
class FISMemberFunction {
private:
    int n_; /**< TODO */
    FISCoords x_, y_; /**< TODO */
    QString msg_; /**< TODO */
    /**
     * @brief
     *
     * @param QString
     * @return bool
     */
    bool setError(QString);
    friend class FISMemberFunctionSet;
protected:
    /**
     * @brief
     *
     * @param x1
     * @param x2
     * @param x3
     * @param yMax
     * @return bool
     */
    bool init(double x1, double x2, double x3, double yMax);
    /**
     * @brief
     *
     * @param x1
     * @param x2
     * @param x3
     * @param x4
     * @param yMax
     * @return bool
     */
    bool init(double x1, double x2, double x3, double x4, double yMax);
public:
    FISMemberFunction();
    /**
     * @brief
     *
     * @param x1
     * @param x2
     * @param x3
     * @param yMax
     */
    FISMemberFunction(double x1, double x2, double x3, double yMax);
    /**
     * @brief
     *
     * @param x1
     * @param x2
     * @param x3
     * @param x4
     * @param yMax
     */
    FISMemberFunction(double x1, double x2, double x3, double x4, double yMax);
    /**
     * @brief
     *
     * @param x
     * @param y
     */
    FISMemberFunction(FISCoords x, FISCoords y);
    /**
     * @brief
     *
     * @param std::map<double
     * @param coords
     */
    FISMemberFunction(std::map<double, double> coords);
    /**
     * @brief
     *
     */
    /**
     * @brief
     *
     */
    void clear();
    /**
     * @brief
     *
     * @param x
     * @return double
     */
    double fuzzify(double x);
    /**
     * @brief
     *
     * @return const char
     */
    const char* getMsg();
    /**
     * @brief
     *
     * @param v1
     * @param v2
     * @param y
     * @return double
     */
    double getX(int v1, int v2, double y);
    /**
     * @brief
     *
     */
    void print();
    /**
     * @brief
     *
     * @return bool
     */
    bool valid();
    /**
     * @brief
     *
     * @param inMf
     * @param outMf
     * @param n
     * @param weight
     */
    static void FISImpMin(FISMemberFunction* inMf, FISMemberFunction* outMf, double n, double weight);
    /**
     * @brief
     *
     * @param inMf
     * @param outMf
     * @param n
     * @param weight
     */
    static void FISImpProduct(FISMemberFunction* inMf, FISMemberFunction* outMf, double n,
                                                        double weight);
    /**
     * @brief
     *
     * @param inMf
     * @param outMf
     */
    static void FISAggMax(FISMemberFunction* inMf, FISMemberFunction* outMf);
    /**
     * @brief
     *
     * @param inMf
     * @param outMf
     */
//        static void FISAggProbor(FISMemberFunction* inMf, FISMemberFunction* outMf); //write!
    /**
     * @brief
     *
     * @param inMf
     * @param outMf
     */
//        static void FISAggSum(FISMemberFunction* inMf, FISMemberFunction* outMf); //write!
    /**
     * @brief
     *
     * @param mf
     * @return double
     */
    static double FISDefuzzCentroid(FISMemberFunction* mf);
    /**
     * @brief
     *
     * @param mf
     * @return double
     */
    static double FISDefuzzBisect(FISMemberFunction* mf); //test!
    /**
     * @brief
     *
     * @param mf
     * @return double
     */
    static double FISDefuzzMidMax(FISMemberFunction* mf); //test!
    /**
     * @brief
     *
     * @param mf
     * @return double
     */
    static double FISDefuzzLargeMax(FISMemberFunction* mf); //test!
    /**
     * @brief
     *
     * @param mf
     * @return double
     */
    static double FISDefuzzSmallMax(FISMemberFunction* mf); //test!

};

/**
 * @brief
 *
 */
class FISMemberFunctionSet {
private:
    int n_; /**< TODO */
    double min_, max_; /**< TODO */
    std::vector<FISMemberFunction> mfs_; /**< TODO */
    std::map<std::string, int> indices_; /**< TODO */
    QString msg_; /**< TODO */
    /**
     * @brief
     *
     * @param QString
     * @return bool
     */
    bool setError(QString);
    friend class FISRuleSet;
public:
    FISMemberFunctionSet();
    /**
     * @brief
     *
     * @param min
     * @param max
     */
    FISMemberFunctionSet(double min, double max);
    /**
     * @brief
     *
     * @param name
     * @param mf
     * @return bool
     */
    /**
     * @brief
     *
     */
    bool addMF(const char* name, FISMemberFunction mf);
    /**
     * @brief
     *
     * @param name
     * @param x1
     * @param x2
     * @param x3
     * @param yMax
     * @return bool
     */
    bool addTriangleMF(const char* name, double x1, double x2, double x3, double yMax=1);
    /**
     * @brief
     *
     * @param name
     * @param x1
     * @param x2
     * @param x3
     * @param x4
     * @param yMax
     * @return bool
     */
    bool addTrapezoidMF(const char* name, double x1, double x2, double x3, double x4, double yMax=1);
    /**
     * @brief
     *
     * @return const char
     */
    const char* getMsg();
    /**
     * @brief
     *
     * @return bool
     */
    bool valid();
};

/**
 * @brief
 *
 */
class FISRuleSet {
private:
    int nInputs_, nRules_; /**< TODO */
    std::vector<FISMemberFunctionSet> inputs_; /**< TODO */
    std::map<std::string, int> indices_; /**< TODO */
    FISMemberFunctionSet output_; /**< TODO */
    QString outputName_; /**< TODO */
    QString msg_; /**< TODO */
    /**
     * @brief
     *
     * @param n1
     * @param n2
     * @return double
     */
    double (*andOperator_) (double n1, double n2);
    /**
     * @brief
     *
     * @param n1
     * @param n2
     * @return double
     */
    double (*orOperator_) (double n1, double n2);
    /**
     * @brief
     *
     * @param inMf
     * @param outMf
     * @param n
     * @param weight
     */
    void (*implicator_) (FISMemberFunction* inMf, FISMemberFunction* outMf, double n, double weight);
    /**
     * @brief
     *
     * @param inMf
     * @param outMf
     */
    void (*aggregator_) (FISMemberFunction* inMf, FISMemberFunction* outMf);
    /**
     * @brief
     *
     * @param mf
     * @return double
     */
    double (*defuzzifier_) (FISMemberFunction* mf);
    /**
     * @brief
     *
     */
    class FISRule {
    public:
        int n_; /**< TODO */
        double weight_; /**< TODO */
        /**
         * @brief
         *
         * @param v1
         * @param v2
         * @return double
         */
        double (*operator_) (double v1, double v2);
        std::vector<int> inputs_; /**< TODO */
        std::vector<int> mfs_; /**< TODO */
        FISMemberFunction* output_; /**< TODO */
        FISRule();
        /**
         * @brief
         *
         * @param inputIndex
         */
        void addMf(int inputIndex, int mfIndex);
    };
    std::vector<FISRule> rules_; /**< TODO */
    std::vector<FISValues> fuzzyInputs_; /**< TODO */
    /**
     * @brief
     *
     * @param rule
     * @param i
     * @param name
     * @return bool
     */
    bool addMFToRule(FISRule* rule, int i, std::string name);
    /**
     * @brief
     *
     * @param fisFile
     * @param line
     * @return bool
     */
    bool parseInput(std::ifstream& fisFile, std::string& line);
    /**
     * @brief
     *
     * @param fisFile
     * @param line
     * @param isInput
     * @param inputNum
     * @return bool
     */
    bool parseInputOutput(std::ifstream& fisFile, std::string &line, bool isInput, int inputNum);
    /**
     * @brief
     *
     * @param fisFile
     * @param line
     * @return bool
     */
    bool parseOutput(std::ifstream& fisFile, std::string& line);
    /**
     * @brief
     *
     * @param fisFile
     * @param line
     * @return bool
     */
    bool parseRules(std::ifstream& fisFile, std::string& line);
    /**
     * @brief
     *
     * @param fisFile
     * @param line
     * @return bool
     */
    bool parseSystem(std::ifstream& fisFile, std::string& line);
    /**
     * @brief
     *
     * @param msg
     * @return bool
     */
    bool setError(QString msg);
    bool setInvalidInput(std::string line);
protected:
    /**
     * @brief
     *
     * @param andOperator
     * @param orOperator
     * @param implicator
     * @param aggregator
     * @param defuzzifier
     */
    void init(FISOperatorAnd andOperator, FISOperatorOr orOperator, FISImplicator implicator,
                        FISAggregator aggregator, FISDefuzzifier defuzzifier);
public:
    FISRuleSet();
    /**
     * @brief
     *
     * @param andOperator
     * @param orOperator
     * @param implicator
     * @param aggregator
     * @param defuzzifier
     */
    FISRuleSet(FISOperatorAnd andOperator, FISOperatorOr orOperator, FISImplicator implicator,
                         FISAggregator aggregator, FISDefuzzifier defuzzifier);
    /**
     * @brief
     *
     * @param name
     * @param mfs
     * @return bool
     */
    bool addInputMFSet(const char* name, FISMemberFunctionSet mfs);
     /**
      * @brief
      *
      * @return int
     */
    void addOutputMFSet(const char* name, FISMemberFunctionSet mfs);
    /**
     * @brief
     *
     * @param inputs
     * @param output
     * @param op
     * @param weight
     * @return bool
     */
    bool addRule(const char* inputs, const char* output, FISOperator op, double weight=1);
    /**
     * @brief
     *
     * @param inputs
     * @return double
     */
    double calculate(std::vector<double> inputs);
    /**
     * @brief
     *
     */
    void initFuzzy(void);
    /**
     * @brief
     *
     * @param dataArrays
     * @param n
     * @param checkNoData
     * @param noDataValues
     * @param noData
     * @return double
     */
    double calculate(std::vector<double> &dataArrays, bool checkNoData,
                     std::vector<double> &noDataValues, double noData);

    /**
     * @brief
     *
     * @param i
     * @return const char
     */
    const char* getInputName(int i);
    /**
     * @brief
     *
     * @return const char
     */
    const char* getMsg();
    /**
     * @brief
     *
     * @return int
     */
    int numInputs();
    /**
     * @brief
     *
     * @param fn
     * @return bool
     */
    bool parseFile(QString fn);
    /**
     * @brief
     *
     * @param index
     * @param name
     * @param mfs
     * @return bool
     */
    bool setInputMFSet(int index, const char* name, FISMemberFunctionSet mfs);
    /**
     * @brief
     *
     * @return bool
     */
    bool valid();

};

}

#endif // FISCORE_H
