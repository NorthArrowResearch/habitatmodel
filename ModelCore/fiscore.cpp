#include "fiscore.h"
#include "benchmark.h"

namespace HabitatModel {

/****************************************************************************************************/
/********************************     FISMemberFunction Private     *********************************/
/****************************************************************************************************/

/**
     * Convenience function for setting the objects error message and returning false.
     * @param msg The error message to set.
     * @return false
     */
bool FISMemberFunction::setError(QString msg) {
    msg_ = msg;
    // TODO: Write to log file a message about what's going on.
    return false;
}

/****************************************************************************************************/
/*******************************     FISMemberFunction Protected     ********************************/
/****************************************************************************************************/

/**
     * Initializes an object with a triangle member function.
     * The x coordinates must be in order from smallest to largest. The y values corresponding to x1 and
     * x3 are assumed to be 0.
     * @param x1 The first x coordinate.
     * @param x2 The second x coordinate.
     * @param x3 The third x coordinate.
     * @param yMax The y value at x2. Must be in the interval (0,1].
     * @return True if successful, false otherwise.
     */
bool FISMemberFunction::init(double x1, double x2, double x3, double yMax) {
    if ((yMax <= 0) || (yMax > 1))
        return setError(QString("Invalid yMax of %1. It must be between 0 and 1.").arg(yMax));
    else if ((x1 > x2) || (x2 > x3))
        return setError(QString("Membership function vertices (%1 %2 %3) must be in ascending order.")
                        .arg(x1).arg(x2).arg(x3) );
    else {
        x_.resize(3);
        x_[0] = x1;
        x_[1] = x2;
        x_[2] = x3;
        y_.resize(3);
        y_[0] = 0;
        y_[1] = yMax;
        y_[2] = 0;
        return true;
    }

}

/**
     * Initializes an object with a trapezoidal member function.
     * The x coordinates must be in order from smallest to largest. The y values corresponding to x1 and
     * x4 are assumed to be 0.
     * @param x1 The first x coordinate.
     * @param x2 The second x coordinate.
     * @param x3 The third x coordinate.
     * @param x4 The fourth x coordinate.
     * @param yMax The y value at x2 and x3. Must be in the interval (0,1].
     * @return True if successful, false otherwise.
     */
bool FISMemberFunction::init(double x1, double x2, double x3, double x4, double yMax) {
    if ((yMax <= 0) || (yMax > 1))
        return setError(QString("Invalid yMax of %1. It must be between 0 and 1.").arg(yMax));
    else if ((x1 > x2) || (x2 >= x3) || (x3 > x4))
        return setError(QString("Membership function vertices (%1 %2 %3 %4) must be in ascending order.")
                        .arg(x1).arg(x2).arg(x3).arg(x4) );
    else {
        x_.resize(4);
        x_[0] = x1;
        x_[1] = x2;
        x_[2] = x3;
        x_[3] = x4;
        y_.resize(4);
        y_[0] = 0;
        y_[1] = yMax;
        y_[2] = yMax;
        y_[3] = 0;
        return true;
    }
}

/****************************************************************************************************/
/*********************************     FISMemberFunction Public     *********************************/
/****************************************************************************************************/

/**
     * Blank constructor.
     * @throw FISException
     */
FISMemberFunction::FISMemberFunction() : n_(0), msg_("") {}

/**
     * Constructor, using a triangle membership function.
     * The x coordinates must be in order from smallest to largest. The y values corresponding to x1 and
     * x3 are assumed to be 0.
     * @param x1 The first x coordinate.
     * @param x2 The second x coordinate.
     * @param x3 The third x coordinate.
     * @param yMax The y value at x2. Must be in the interval (0,1].
     * @throw FISException
     */
FISMemberFunction::FISMemberFunction(double x1, double x2, double x3, double yMax) : n_(3), msg_("") {
    if (!init(x1, x2, x3, yMax))
        throw FISException(msg_);
}

/**
     * Constructor, using a trapezoidal membership function.
     * The x coordinates must be in order from smallest to largest. The y values corresponding to x1 and
     * x4 are assumed to be 0.
     * @param x1 The first x coordinate.
     * @param x2 The second x coordinate.
     * @param x3 The third x coordinate.
     * @param x4 The fourth x coordinate.
     * @param yMax The y value at x2 and x3. Must be in the interval (0,1].
     * @throw FISException
     */
FISMemberFunction::FISMemberFunction(double x1, double x2, double x3, double x4, double yMax) : n_(4), msg_("") {
    if (!init(x1, x2, x3, x4, yMax))
        throw FISException(msg_);
}

/**
     * Constructor, using two vectors of coordinates.
     * The x coordinates must be in order from smallest to largest. The x and y coordinates must be in the
     * same order.
     * @param x The vector of x coordinates.
     * @param y The vector of y coordinates.
     * @throw FISException
     */
FISMemberFunction::FISMemberFunction(FISCoords x, FISCoords y) :
    n_(x.size()), x_(x), y_(y), msg_("") {
    if (x.size() != y.size())
        throw FISException("x and y are different sizes.");
    else if (0 == x.size())
        throw FISException("No coordinates provided.");
}

/**
     * Constructor, using a map of x and y coordinates.
     * The keys are the x coordinates and the values or the corresponding y coordinates.
     * @param coords The map of coordinates.
     * @throw FISException
     */
FISMemberFunction::FISMemberFunction(std::map<double, double> coords) :
    n_(coords.size()), msg_("") {
    if (0 == coords.size())
        throw FISException("No coordinates provided.");
    for (std::map<double, double>::iterator it=coords.begin(); it!=coords.end(); it++) {
        x_.push_back((*it).first);
        y_.push_back((*it).second);
    }
}

/**
     * Clear all vertices out of this function.
     */
void FISMemberFunction::clear() {
    x_.clear();
    y_.clear();
    n_ = 0;
}

/**
     * Gets the y value from a membership function, given an x value.
     * @param x The x coordinate to get the corresponding y value for.
     * @return The y coordinate.
     */
double FISMemberFunction::fuzzify(double x) {
    double result = 0;
    for (unsigned i=1; i<x_.size(); i++) {
        if ((x >= x_[i-1]) && (x <= x_[i])) {
            result = y_[i-1] + (x - x_[i-1]) * ((y_[i] - y_[i-1]) / (x_[i] - x_[i-1]));
            break;
        }
    }
    return result;
}

/**
     * Get the objects error message.
     * @return The error message.
     */
const char* FISMemberFunction::getMsg() {
    const QByteArray qbReturn = msg_.toLocal8Bit();
    return qbReturn.data();
}

/**
     * Computes the x coordinate that corresponds to a given y coordinate in between two vertices.
     * @param v1 The index of the first vertex.
     * @param v2 The index of the second vertex.
     * @param y The y coordinate.
     * @return The x coordinate.
     */
double FISMemberFunction::getX(int v1, int v2, double y) {
    return (x_[v2] - x_[v1]) / (y_[v2] - y_[v1]) * (y - y_[v1]) + x_[v1];
}

/**
     * Print a membership function.
     */
void FISMemberFunction::print() {
    for (int i=0; i<n_; i++)
        std::cout << "(" << x_[i] << "," << y_[i] << ") ";
    std::cout << std::endl;
}

/**
     * Check if a membership function is valid.
     */
bool FISMemberFunction::valid() {
    return n_ > 0;
}

/**
     * Apply the Minimum implication method to a membership function.
     * This is a static function that does not apply to a specific object.
     * @param inMf The membership function to implicate.
     * @param outMf The membership function to hold the output.
     * @param n The antecedent to reshape the membership function with.
     * @param weight The weight of the rule.
     */
void FISMemberFunction::FISImpMin(FISMemberFunction* inMf, FISMemberFunction* outMf, double n,
                                  double weight) {
    outMf->clear();
    outMf->x_.push_back(inMf->x_[0]);
    outMf->y_.push_back(inMf->y_[0] * weight);
    int i,j;
    for (i=1; i< inMf->n_-1; i++) {
        if (inMf->y_[i] > n) {
            outMf->x_.push_back(inMf->getX(i-1, i, n));
            outMf->y_.push_back(n * weight);
            break;
        } else {
            outMf->x_.push_back(inMf->x_[i]);
            outMf->y_.push_back(inMf->y_[i] * weight);
        }
    }
    for (j=i; j<inMf->n_; j++) {
        if (inMf->y_[j] < n) {
            outMf->x_.push_back(inMf->getX(j-1, j, n));
            outMf->y_.push_back(n * weight);
            outMf->x_.push_back(inMf->x_[j]);
            outMf->y_.push_back(inMf->y_[j] * weight);
        }
    }
    outMf->n_ = outMf->x_.size();
}

/**
     * Apply the Product implication method to a membership function.
     * This is a static function that does not apply to a specific object.
     * @param inMf The membership function to implicate.
     * @param outMf The membership function to hold the output.
     * @param n The antecedent to reshape the membership function with.
     * @param weight The weight of the rule.
     */
void FISMemberFunction::FISImpProduct(FISMemberFunction* inMf, FISMemberFunction* outMf, double n,
                                      double weight) {
    outMf->clear();
    for (int i=0; i<inMf->n_; i++) {
        outMf->n_ = inMf->n_;
        outMf->x_.push_back(inMf->x_[i]);
        outMf->y_.push_back(inMf->y_[i] * n * weight);
    }
    outMf->n_ = outMf->x_.size();
}

/**
     * Aggregate two membership functions using the Maximum method.
     * This is a static function that does not apply to a specific object.
     * @param inMf One of the membership functions to aggregate. This one will not be modified.
     * @param outMf The other membership function to aggregate. This one will be modified to hold the
     *              output function.
     */
void FISMemberFunction::FISAggMax(FISMemberFunction* inMf, FISMemberFunction* outMf) {
    if (0 == outMf->n_) {
        *outMf = *inMf;
    } else {
        std::map<double, double> coords;
        double x, y;
        int i, j;
        for (i=0; i<inMf->n_; i++)
            coords[inMf->x_[i]] = std::max(coords[inMf->x_[i]], std::max(inMf->y_[i], outMf->fuzzify(inMf->x_[i])));
        for (i=0; i<outMf->n_; i++)
            coords[outMf->x_[i]] = std::max(coords[outMf->x_[i]], std::max(inMf->fuzzify(outMf->x_[i]), outMf->y_[i]));
        for (i=1; i<inMf->n_; i++) {
            for (j=1; j<outMf->n_; j++) {
                if (FISIntersectLines(inMf->x_[i-1], inMf->y_[i-1], inMf->x_[i], inMf->y_[i], outMf->x_[j-1],
                                      outMf->y_[j-1], outMf->x_[j], outMf->y_[j], &x, &y))
                    coords[x] = std::max(coords[x], y);
            }
        }
        outMf->clear();
        std::map<double, double>::iterator it=coords.begin();
        if (0 != (*it).second) {
            outMf->x_.push_back((*it).first);
            outMf->y_.push_back(0);
        }
        for (it=it; it!=coords.end(); it++) {
            outMf->x_.push_back((*it).first);
            outMf->y_.push_back((*it).second);
        }
        it--;
        if (0 != (*it).second) {
            outMf->x_.push_back((*it).first);
            outMf->y_.push_back(0);
        }
        outMf->n_ = outMf->x_.size();
    }
}


/**
     * Defuzzify a membership function using the Centroid method.
     * This is a static function that does not apply to a specific object.
     * @param mf The membership function to defuzzify.
     * @return The crisp result of defuzzification.
     */
double FISMemberFunction::FISDefuzzCentroid(FISMemberFunction* mf) {
    double area = 0;
    double xCentroid = 0;
    double m;
    for (int i=0; i<mf->n_-1; i++) {
        m = mf->x_[i] * mf->y_[i+1] - mf->x_[i+1] * mf->y_[i];
        area += m;
        xCentroid += (mf->x_[i] + mf->x_[i+1]) * m;
    }
    int i = mf->n_ - 1;
    m = mf->x_[i] * mf->y_[0] - mf->x_[0] * mf->y_[i];
    area += m;
    xCentroid += (mf->x_[i] + mf->x_[0]) * m;
    area = area / 2;
    xCentroid = xCentroid / (6 * area);
    return xCentroid;
}

/**
     * Defuzzify a membership function using the Bisector method.
     * This is a static function that does not apply to a specific object.
     * @param mf The membership function to defuzzify.
     * @return The crisp result of defuzzification.
     */
double FISMemberFunction::FISDefuzzBisect(FISMemberFunction* mf) {
    double area = 0;
    for (int i=0; i<mf->n_-1; i++)
        area += mf->x_[i] * mf->y_[i+1] - mf->x_[i+1] * mf->y_[i];
    area += mf->x_[mf->n_-1] * mf->y_[0] - mf->x_[0] * mf->y_[mf->n_-1];
    area = area / 2;
    double xMin, xMax, yLast, x, y, m, tmpArea = 0;
    for (int i=0; i<mf->n_-1; i++) {
        m = mf->x_[i] * mf->y_[i+1] - mf->x_[i+1] * mf->y_[i];
        if (tmpArea + m < area) {
            tmpArea += m;
        } else {
            xMin = mf->x_[i];
            xMax = mf->x_[i+1];
            yLast = mf->y_[i];
            x = xMax;
            while (area - tmpArea > 0.000001) {
                x = (xMax - xMin) / 2;
                y = mf->fuzzify(x);
                m = xMin * y - x * yLast;
                if (tmpArea + m > area) {
                    xMax = x;
                } else if (tmpArea + m <= area) {
                    tmpArea += m;
                    xMin = x;
                }
            }
        }
    }
    return x;
}

/**
     * Defuzzify a membership function using the Middle of Maximum method.
     * This is a static function that does not apply to a specific object.
     * @param mf The membership function to defuzzify.
     * @return The crisp result of defuzzification.
     */
double FISMemberFunction::FISDefuzzMidMax(FISMemberFunction* mf) {
    double minX = mf->x_[0];
    double maxX = mf->x_[0];
    double y = mf->y_[0];
    for (int i=1; i<mf->n_; i++) {
        if (mf->y_[i] > y) {
            minX = mf->x_[i];
            maxX = minX;
            y = mf->y_[i];
        } else if (mf->y_[i] == y) {
            maxX = mf->x_[i];
        }
    }
    return (maxX - minX) / 2;
}

/**
     * Defuzzify a membership function using the Largest of Maximum method.
     * This is a static function that does not apply to a specific object.
     * @param mf The membership function to defuzzify.
     * @return The crisp result of defuzzification.
     */
double FISMemberFunction::FISDefuzzLargeMax(FISMemberFunction* mf) {
    double x = mf->x_[0];
    double y = mf->y_[0];
    for (int i=1; i<mf->n_; i++) {
        if (mf->y_[i] >= y) {
            x = mf->x_[i];
            y = mf->y_[i];
        }
    }
    return x;
}

/**
     * Defuzzify a membership function using the Smallest of Maximum method.
     * This is a static function that does not apply to a specific object.
     * @param mf The membership function to defuzzify.
     * @return The crisp result of defuzzification.
     */
double FISMemberFunction::FISDefuzzSmallMax(FISMemberFunction* mf) {
    double x = mf->x_[0];
    double y = mf->y_[0];
    for (int i=1; i<mf->n_; i++) {
        if (mf->y_[i] > y) {
            x = mf->x_[i];
            y = mf->y_[i];
        }
    }
    return x;
}

/****************************************************************************************************/
/********************************     FISMemberFunctionSet Private     ******************************/
/****************************************************************************************************/

/**
     * Convenience function for setting the objects error message and returning false.
     * @param msg The error message to set.
     * @return false
     */
bool FISMemberFunctionSet::setError(QString msg) {
    msg_ = msg;
    return false;
}

/****************************************************************************************************/
/********************************     FISMemberFunctionSet Public     *******************************/
/****************************************************************************************************/

/**
     * Blank constructor.
     * @throw FISException
     */
FISMemberFunctionSet::FISMemberFunctionSet() : n_(0), msg_("") {}

/**
     * Constructor.
     * @param min The minimum allowable x value.
     * @param max The maximum allowable x value.
     * @throw FISException
     */
FISMemberFunctionSet::FISMemberFunctionSet(double min, double max) :
    n_(0), min_(min), max_(max), msg_("") {
    if (min >= max)
        throw FISException("Invalid range. Max must be greater than min.");
}

/**
     * Add a member function to the set.
     * @param name The name of the member function. Cannot contain spaces.
     * @param mf The member function to add.
     * @return True if successful, false otherwise.
     */
bool FISMemberFunctionSet::addMF(const char* name, FISMemberFunction mf) {
    if (0 == mf.n_)
        return setError("The membership function cannot be added to the set because it has no vertices.");
    else if ((mf.x_[0] < min_) || (mf.x_[mf.n_ - 1] > max_))
        return setError(QString("Membership function bounds (%1 %2) do not fit in the set range (%3 %4) for this object.")
                        .arg(mf.x_[0]).arg(mf.x_[mf.n_-1]).arg(min_).arg(max_));
    else if (indices_.end() != indices_.find(name))
        return setError(QString("The name '%1' is already in use.").arg(name));
    else if (std::string::npos != std::string(name).find(' '))
        return setError(QString("Invalid name '%1'. Spaces are not allowed.").arg(name));
    else {
        mfs_.push_back(mf);
        indices_[name] = n_;
        n_++;
        return true;
    }
}

/**
     * Add a triangle member function to the set.
     * The x coordinates must be in order from smallest to largest. The y values corresponding to x1 and
     * x3 are assumed to be 0.
     * @param name The name of the member function. Cannot contain spaces.
     * @param x1 The first x coordinate.
     * @param x2 The second x coordinate.
     * @param x3 The third x coordinate.
     * @param yMax The y value at x2. Must be in the interval (0,1]. (Optional, defaults to 1.)
     * @return True if successful, false otherwise.
     */
bool FISMemberFunctionSet::addTriangleMF(const char* name, double x1, double x2, double x3,
                                         double yMax) {
    if ((x1 < min_) || (x3 > max_))
        return setError(QString("Membership function bounds (%1 %2) do not fit in the set range (%3 %4) for this object.")
                .arg(x1).arg(x3).arg(min_).arg(max_));
    else if (indices_.end() != indices_.find(name))
        return setError(QString("The name '%1' is already in use.").arg(name));
    else if (std::string::npos != std::string(name).find(' '))
        return setError(QString("Invalid name '%1'. Spaces are not allowed.").arg(name));
    else {
        try {
            mfs_.push_back(FISMemberFunction(x1, x2, x3, yMax));
            indices_[name] = n_;
            n_++;
            return true;
        } catch (FISException& e) {
            return setError(e.what());
        }
    }
}

/**
     * Add a trapezoidal member function to the set.
     * The x coordinates must be in order from smallest to largest. The y values corresponding to x1 and
     * x4 are assumed to be 0.
     * @param name The name of the member function. Cannot contain spaces.
     * @param x1 The first x coordinate.
     * @param x2 The second x coordinate.
     * @param x3 The third x coordinate.
     * @param x4 The fourth x coordinate.
     * @param yMax The y value at x2 and x3. Must be in the interval (0,1]. (Optional, defaults to 1.)
     * @return True if successful, false otherwise.
     */
bool FISMemberFunctionSet::addTrapezoidMF(const char* name, double x1, double x2, double x3,
                                          double x4, double yMax) {
    if ((x1 < min_) || (x4 > max_))
        return setError(QString("Membership function bounds (%1 %2) do not fit in the set range (%3 %4) for this object.")
                .arg(x1).arg(x4).arg(min_).arg(max_));
    else if (indices_.end() != indices_.find(name))
        return setError(QString("The name '%1' is already in use.").arg(name));
    else if (std::string::npos != std::string(name).find(' '))
        return setError(QString("Invalid name '%1'. Spaces are not allowed.").arg(name));
    else {
        try {
            mfs_.push_back(FISMemberFunction(x1, x2, x3, x4, yMax));
            indices_[name] = n_;
            n_++;
            return true;
        } catch (FISException& e) {
            return setError(e.what());
        }
    }
}

/**
     * Get the objects error message.
     * @return The error message.
     */
const char* FISMemberFunctionSet::getMsg() {
    const QByteArray qbReturn = msg_.toLocal8Bit();
    return qbReturn.data();
}

/**
     * Check if a membership function is valid.
     */
bool FISMemberFunctionSet::valid() {
    if (n_ == 0)
        return false;
    for (int i=0; i<n_; i++)
        if (!mfs_[i].valid())
            return false;
    return true;
}

/****************************************************************************************************/
/*************************************     FISRuleSet Private     ***********************************/
/****************************************************************************************************/

/**
     * Blank constructor.
     */
FISRuleSet::FISRule::FISRule() : n_(0), weight_(1) {}

/**
     * Add an input membership function to a rule.
     * @param inputIndex The index of the input variable.
     * @param mfIndex The index of the membership function (in the input variable).
     */
void FISRuleSet::FISRule::addMf(int inputIndex, int mfIndex) {
    inputs_.push_back(inputIndex);
    mfs_.push_back(mfIndex);
    n_++;
}

/**
     * Add a membership function to a rule.
     * @param rule The rule to add the membership function to.
     * @param i The index of the input variable that the MF corresponds to.
     * @param name The name of the membership function
     * @return True if a MF of the given name exists for that input variable and was added, false
     *         otherwise.
     */
bool FISRuleSet::addMFToRule(FISRule* rule, int i, std::string name) {
    if (inputs_[i].indices_.find(name) == inputs_[i].indices_.end())
        return setError(QString("There is no membership function named '%1'.")
                        .arg(QString::fromStdString(name)));
    else {
        if ("NULL" != name) {
            rule->inputs_.push_back(i);
            rule->mfs_.push_back(inputs_[i].indices_[name]);
            rule->n_++;
        }
        return true;
    }
}

/**
     * Parse an Input section of a .fis file.
     * @param fisFile The input .fis file stream to read.
     * @param line The current line (should be the [Input?] line).
     * @return True if successful, false otherwise.
     */
bool FISRuleSet::parseInput(std::ifstream& fisFile, std::string &line) {
    int n;
    if (!Habitat::string2int(line.substr(6, line.find("]") - 6), n))
        return setInvalidInput(line);
    return parseInputOutput(fisFile, line, true, --n);
}


bool FISRuleSet::setInvalidInput(std::string line) {
    return setError(QString("Invalid input: %1").arg(QString::fromStdString(line)));
}

/**
     * Parse an Input or Output section of a .fis file for the membership functions.
     * @param fisFile The input .fis file stream to read.
     * @param line The current line (should be the [Input?] or [Output?] line).
     * @return True if successful, false otherwise.
     */
bool FISRuleSet::parseInputOutput(std::ifstream& fisFile, std::string &line, bool isInput, int inputNum) {
    std::string inputId, name;
    double low, high;
    std::vector<FISMemberFunction> mfs;
    std::vector<std::string> mfNames;
    std::vector<bool> mfOK;
    std::vector<std::string> tokens;
    int i;
    try {
        getline(fisFile, line);
        while (!fisFile.eof() && (line.length() > 0) && ('[' != line[0])) {
            inputId = line.substr(0, std::min(line.find("="), line.find(" ")));
            std::transform(inputId.begin(), inputId.end(), inputId.begin(), tolower);
            if ("name" == inputId) {
                i = line.find("'");
                name = line.substr(i + 1, line.find("'", i + 1) - i - 1);
            } else if ("range" == inputId) {
                i = line.find("[");
                Habitat::tokenize(line.substr(i + 1, line.rfind("]", i) - i - 1), tokens);
                if ((2 != tokens.size()) || !Habitat::string2double(tokens[0], low) || !Habitat::string2double(tokens[1], high))
                    return setInvalidInput(line);
            } else if ("nummfs" == inputId) {
                if (!Habitat::string2int(line.substr(line.find("=") + 1), i))
                    return setInvalidInput(line);
                mfs.resize(i);
                mfNames.resize(i);
                mfOK.resize(i);
                //for (int n=0; n<i; n++)
                //	mfOK[i] = false;
            } else if ("mf" == inputId.substr(0, 2)) {
                int n;
                if (!Habitat::string2int(inputId.substr(2), n) || (n < 1) || (n > (int)mfs.size()))
                    return setInvalidInput(line);
                n--;
                Habitat::tokenize(line, tokens, "'");
                if (tokens.size() < 5)
                    return setInvalidInput(line);
                mfNames[n] = tokens[1];
                std::string mfType = tokens[3];
                std::transform(mfType.begin(), mfType.end(), mfType.begin(), tolower);
                i = tokens[4].find("[");
                Habitat::tokenize(tokens[4].substr(i + 1, tokens[4].rfind("]") - i - 1), tokens);
                if (0 == tokens.size())
                    return setInvalidInput(line);
                std::vector<double> vertices;
                vertices.resize(tokens.size());
                std::transform(tokens.begin(), tokens.end(), vertices.begin(), Habitat::convertToDouble);
                if ("trapmf" == mfType) {
                    if (4 != tokens.size())
                        return setInvalidInput(line);
                    mfs[n] = FISMemberFunction(vertices[0], vertices[1], vertices[2], vertices[3], 1);
                } else if ("trimf" == mfType) {
                    if (3 != tokens.size())
                        return setInvalidInput(line);
                    mfs[n] = FISMemberFunction(vertices[0], vertices[1], vertices[2], 1);
                } else
                    return setError(QString("%1 is not a supported membership function type.")
                                    .arg(QString::fromStdString(mfType)));
                mfOK[n] = true;
            }
            getline(fisFile, line);
        }
        FISMemberFunctionSet mfSet(low, high);
        for (i=0; i<(int)mfs.size(); i++) {
            if (!mfOK[i])
                return setError(QString("Missing membership function %1 for input %2")
                                .arg(i+2).arg(inputNum));
            if (!mfSet.addMF(mfNames[i].c_str(), mfs[i]))
                return setError(mfSet.getMsg());
        }
        if (isInput) {
            if (!setInputMFSet(inputNum, name.c_str(), mfSet))
                return false;
            //return setError(mfSet.getMsg());
        } else
            addOutputMFSet(name.c_str(), mfSet);
    } catch (FISException e) {
        return setError(e.what());
    }
    return true;
}

/**
     * Parse an Output section of a .fis file.
     * @param fisFile The input .fis file stream to read.
     * @param line The current line (should be the [Output?] line).
     * @return True if successful, false otherwise.
     */
bool FISRuleSet::parseOutput(std::ifstream& fisFile, std::string &line) {
    int n;
    if (!Habitat::string2int(line.substr(7, line.find("]") - 7), n) || (1 != n))
        return setInvalidInput(line);
    return parseInputOutput(fisFile, line, false, 0);
}

/**
     * Parse a Rules section of a .fis file.
     * @param fisFile The input .fis file stream to read.
     * @param line The current line (should be the [Rules] line).
     * @return True if successful, false otherwise.
     */
bool FISRuleSet::parseRules(std::ifstream& fisFile, std::string &line) {
    std::vector<std::string> tokens;
    std::vector<int> indices;
    int i1, i2, mfIndex, nRules = 0;
    indices.resize(nInputs_);
    getline(fisFile, line);
    while (!fisFile.eof() && (line.length() > 0) && ('[' != line[0])) {
        i1 = line.find(",");
        Habitat::tokenize(line.substr(0, i1), tokens);
        if ((int)tokens.size() != nInputs_)
            return setError(QString("Wrong number of inputs: %1").arg(QString::fromStdString(line)));
        std::transform(tokens.begin(), tokens.end(), indices.begin(), Habitat::convertToInt);
        FISRule rule;
        for (int n=0; n<nInputs_; n++) {
            mfIndex = indices[n];
            if (0 != mfIndex) {
                mfIndex--;
                if (0 > mfIndex || mfIndex >= inputs_[n].n_)
                    return setInvalidInput(line);
                rule.addMf(n, mfIndex);
            }
        }
        i2 = line.find("(", i1 + 1);
        Habitat::tokenize(line.substr(i1 + 1, i2 - i1 - 1), tokens);
        if (1 != tokens.size() || !Habitat::string2int(tokens[0], mfIndex))
            return setInvalidInput(line);
        rule.output_ = &output_.mfs_[--mfIndex];
        i1 = line.find(")", i2 + 1);
        if (!Habitat::string2double(line.substr(i2 + 1, i1 - i2 - 1), rule.weight_))
            return setInvalidInput(line);
        if (!Habitat::string2int(line.substr(line.find(":") + 1), mfIndex) || 1 > mfIndex || 2 < mfIndex)
            return setInvalidInput(line);
        if (1 == mfIndex)
            rule.operator_ = andOperator_;
        else if (2 == mfIndex)
            rule.operator_ = orOperator_;
        rules_.push_back(rule);
        nRules++;
        getline(fisFile, line);
    }
    if (nRules == nRules_)
        return true;
    else
        return setError("Wrong number of rules.");
}

/**
     * Parse a System section of a .fis file.
     * @param fisFile The input .fis file stream to read.
     * @param line The current line (should be the [System] line).
     * @return True if successful, false otherwise.
     */
bool FISRuleSet::parseSystem(std::ifstream& fisFile, std::string &line) {
    std::vector<std::string> tokens;
    std::string inputId;
    FISOperatorAnd andOp = FISOpAnd_None;
    FISOperatorOr orOp = FISOpOr_None;
    FISImplicator imp = FISImp_None;
    FISAggregator agg = FISAgg_None;
    FISDefuzzifier defuzz = FISDefuzz_None;
    int n;
    getline(fisFile, line);
    while (!fisFile.eof() && (line.length() > 0) && ('[' != line[0])) {
        Habitat::tokenize(line, tokens, "=");
        if (0 < tokens.size()) {
            inputId = tokens[0];
            std::transform(inputId.begin(), inputId.end(), inputId.begin(), tolower);
            if ("numinputs" == inputId) {
                if (!Habitat::string2int(tokens[1], nInputs_))
                    return setInvalidInput(line);
                inputs_.resize(nInputs_);
            } else if ("numoutputs" == inputId) {
                if (!Habitat::string2int(tokens[1], n) || 1 != n)
                    return setInvalidInput(line);
            } else if ("numrules" == inputId) {
                if (!Habitat::string2int(tokens[1], nRules_))
                    return setInvalidInput(line);
            } else if ("andmethod" == inputId) {
                if ("'min'" == tokens[1])
                    andOp = FISOpAnd_Min;
                else if ("'product'" == tokens[1])  // CHECK!
                    andOp = FISOpAnd_Product;
                else
                    return setError(QString("Unsupported AndMethod: %1").arg(QString::fromStdString(tokens[1])) );
            }	else if ("ormethod" == inputId) {
                if ("'max'" == tokens[1])
                    orOp = FISOpOr_Max;
                else if ("'probor'" == tokens[1])  // CHECK!
                    orOp = FISOpOr_Probor;
                else
                    return setError(QString("Unsupported OrMethod: %1").arg(QString::fromStdString(tokens[1])) );
            }	else if ("impmethod" == inputId) {
                if ("'min'" == tokens[1])
                    imp = FISImp_Min;
                else if ("'product'" == tokens[1])  // CHECK!
                    imp = FISImp_Product;
                else
                    return setError(QString("Unsupported ImpMethod: %1").arg(QString::fromStdString(tokens[1])) );
            }	else if ("aggmethod" == inputId) {
                if ("'max'" == tokens[1])
                    agg = FISAgg_Max;
                else if ("'probor'" == tokens[1])  // CHECK!
                    agg = FISAgg_Probor;
                else if ("'sum'" == tokens[1])  // CHECK!
                    agg = FISAgg_Sum;
                else
                    return setError(QString("Unsupported AggMethod: %1").arg(QString::fromStdString(tokens[1])) );
            }	else if ("defuzzmethod" == inputId) {
                //			op = line.substr(line.find("=") + 1);
                if ("'centroid'" == tokens[1])
                    defuzz = FISDefuzz_Centroid;
                else if ("'bisect'" == tokens[1])  // CHECK!
                    defuzz = FISDefuzz_Bisect;
                else if ("'midmax'" == tokens[1])  // CHECK!
                    defuzz = FISDefuzz_MidMax;
                else if ("'largemax'" == tokens[1])  // CHECK!
                    defuzz = FISDefuzz_LargeMax;
                else if ("'smallmax'" == tokens[1])  // CHECK!
                    defuzz = FISDefuzz_SmallMax;
                else
                    return setError(QString("Unsupported DefuzzMethod: %1").arg(QString::fromStdString(tokens[1])));
            }
        }
        getline(fisFile, line);
    }
    if (andOp == FISOpAnd_None)
        return setError("No AndMethod set.");
    else if (orOp == FISOpOr_None)
        return setError("No OrMethod set.");
    else if (imp == FISImp_None)
        return setError("No ImpMethod set.");
    else if (agg == FISAgg_None)
        return setError("No AggMethod set.");
    else if (defuzz == FISDefuzz_None)
        return setError("No DefuzzMethod set.");
    init(andOp, orOp, imp, agg, defuzz);
    return true;
}

/**
     * Convenience function for setting the objects error message and returning false.
     * @param msg The error message to set.
     * @return false
     */
bool FISRuleSet::setError(QString msg) {
    msg_ = msg;
    return false;
}

/****************************************************************************************************/
/************************************     FISRuleSet Protected     **********************************/
/****************************************************************************************************/

/**
     * Initializes an object.
     * @param andOperator The fuzzy AND operator to use for rules in this set.
     * @param orOperator The fuzzy OR operator to use for rules in this set.
     * @param implicator The implication method to use for rules in this set.
     * @param aggregator The aggregation method to use for rules in this set.
     * @param defuzzifier The defuzzify method to use to obtain a crisp output.
     */
void FISRuleSet::init(FISOperatorAnd andOperator, FISOperatorOr orOperator, FISImplicator implicator,
                      FISAggregator aggregator, FISDefuzzifier defuzzifier) {
    if (FISOpAnd_Min == andOperator)
        andOperator_ = FISOpMin;
    else if (FISOpAnd_Product == andOperator)
        andOperator_ = FISOpProduct;
    if (FISOpOr_Max == orOperator)
        orOperator_ = FISOpMax;
    else if (FISOpOr_Probor == orOperator)
        orOperator_ = FISOpProbor;
    if (FISImp_Min == implicator)
        implicator_ = FISMemberFunction::FISImpMin;
    else if (FISImp_Product == implicator)
        implicator_ = FISMemberFunction::FISImpProduct;
    if (FISAgg_Max == aggregator)
        aggregator_ = FISMemberFunction::FISAggMax;
//    else if (FISAgg_Probor == aggregator)
//        aggregator_ = FISMemberFunction::FISAggProbor;
//    else if (FISAgg_Sum == aggregator)
//        aggregator_ = FISMemberFunction::FISAggSum;
    if (FISDefuzz_Centroid == defuzzifier)
        defuzzifier_ = FISMemberFunction::FISDefuzzCentroid;
    else if (FISDefuzz_Bisect == defuzzifier)
        defuzzifier_ = FISMemberFunction::FISDefuzzBisect;
    else if (FISDefuzz_MidMax == defuzzifier)
        defuzzifier_ = FISMemberFunction::FISDefuzzMidMax;
    else if (FISDefuzz_LargeMax == defuzzifier)
        defuzzifier_ = FISMemberFunction::FISDefuzzLargeMax;
    else if (FISDefuzz_SmallMax == defuzzifier)
        defuzzifier_ = FISMemberFunction::FISDefuzzSmallMax;
}

/****************************************************************************************************/
/*************************************     FISRuleSet Public     ************************************/
/****************************************************************************************************/

/**
     * Blank constructor.
     * @throw FISException
     */
FISRuleSet::FISRuleSet() : nInputs_(0), nRules_(0), msg_("")  {}

/**
     * Constructor.
     * @param andOperator The fuzzy AND operator to use for rules in this set.
     * @param orOperator The fuzzy OR operator to use for rules in this set.
     * @param implicator The implication method to use for rules in this set.
     * @param aggregator The aggregation method to use for rules in this set.
     * @param defuzzifier The defuzzify method to use to obtain a crisp output.
     * @throw FISException
     */
FISRuleSet::FISRuleSet(FISOperatorAnd andOperator, FISOperatorOr orOperator, FISImplicator implicator,
                       FISAggregator aggregator, FISDefuzzifier defuzzifier) :
    nInputs_(0), nRules_(0), msg_("") {
    init(andOperator, orOperator, implicator, aggregator, defuzzifier);
}

/**
     * Add a set of membership functions that make up an input variable.
     * @param name The name of the input variable. Cannot contain spaces.
     * @param mfs The membership function set.
     * @return True of the set could be added, false otherwise.
     */
bool FISRuleSet::addInputMFSet(const char* name, FISMemberFunctionSet mfs) {
    if (indices_.end() != indices_.find(name))
        return setError(QString("The name '%1' is already in use.").arg(name));
    else if (std::string::npos != std::string(name).find(' '))
        return setError(QString("Invalid name '%1'. Spaces are not allowed.").arg(name));
    else {
        inputs_.push_back(mfs);
        indices_[name] = nInputs_;
        nInputs_++;
        return true;
    }
}

/**
     * Add a set of membership functions that make up the output variable.
     * @param mfs The membership function set.
     */
void FISRuleSet::addOutputMFSet(const char* name, FISMemberFunctionSet mfs) {
    output_ = mfs;
    outputName_ = name;
}

/**
     * Add a rule.
     * The inputs MUST be in the same order as they were added to the Rule Set. Use "NULL" to denote that
     * a specific input isn't need for this rule.
     * @param inputs The relevant member function names for each input, separated by spaces.
     * @param output The member function name for the output.
     * @param op The operator function to use.
     * @param weight The rule weight (optional, defaults to 1).
     * @return True if successful, false otherwise.
     */
bool FISRuleSet::addRule(const char* inputs, const char* output, FISOperator op, double weight) {
    if (weight < 0 || weight > 1)
        return setError(QString("The weight must be between 0 and 1."));
    else if (output_.indices_.find(output) == output_.indices_.end())
        return setError(QString("There is no output membership function named '%1'.").arg(output));
    else {
        FISRule rule;
        rule.weight_ = weight;
        rule.output_ = &output_.mfs_[output_.indices_[output]];
        if (FISOp_And == op)
            rule.operator_ = andOperator_;
        else if (FISOp_Or == op)
            rule.operator_ = orOperator_;
        std::string name, inputStr = std::string(inputs);
        int i, s1 = 0, s2;
        rule.n_ = 0;
        for (i=0; i<nInputs_-1; i++) {
            if ((s1 >= (int)inputStr.size()) || ((int)std::string::npos == (s2 = inputStr.find(' ', s1))))
                return setError("There are not enough membership functions provided for the rule.");
            else {
                name = inputStr.substr(s1, s2 - s1);
                if (!addMFToRule(&rule, i, name))
                    return false;
                else
                    s1 = s2 + 1;
            }
        }
        if (s1 >= (int)inputStr.size())
            return setError("There are not enough membership functions provided for the rule.");
        name = inputStr.substr(s1);
        if (!addMFToRule(&rule, i, name))
            return false;
        else {
            rules_.push_back(rule);
            nRules_++;
        }
        return true;
    }
}

/**
     * Calculate a crisp output based on a set of inputs to this rule set.
     * @param inputs The list of input values. These MUST be in the same order that the input variables
     *               were added to the rule set.
     * @return The output value.
     */
double FISRuleSet::calculate(std::vector<double> inputs) {
    if ((int)inputs.size() != nInputs_)
        return -1;
    if (0 == fuzzyInputs_.size()) { // DO THIS PART IN MAIN FUNCTION (ARRAYS)
        fuzzyInputs_.resize(nInputs_);
        for (int i=0; i<nInputs_; i++)
            fuzzyInputs_[i].resize(inputs_[i].n_);
    }
    int i, j;
    for (i=0; i<nInputs_; i++)
        for (j=0; j<inputs_[i].n_; j++)
            fuzzyInputs_[i][j] = inputs_[i].mfs_[j].fuzzify(inputs[i]);
    FISMemberFunction impMf, aggMf;
    for (int r=0; r<nRules_; r++) {
        FISRule rule = rules_[r];
        double (*op) (double v1, double v2) = rule.operator_;
        double impValue = fuzzyInputs_[rule.inputs_[0]][rule.mfs_[0]];
        for (int i=1; i<rule.n_; i++)
            impValue = op(impValue, fuzzyInputs_[rule.inputs_[i]][rule.mfs_[i]]);
        implicator_(rule.output_, &impMf, impValue, rule.weight_);
        aggregator_(&impMf, &aggMf);
    }
    return defuzzifier_(&aggMf);
}

void FISRuleSet::initFuzzy(void)
{
    fuzzyInputs_.resize(nInputs_);
    for (int i=0; i<nInputs_; i++)
        fuzzyInputs_[i].resize(inputs_[i].n_);
}

/**
     * Apply a FIS to a set of arrays.
     * All data sets are assumed to be doubleing point. The data sets are referenced in the DoDData object
     * with the keywords "old", "new", and "dod". ****** UPDATE *******
     * @param xValid The number of valid pixels in the x direction.
     * @param yValid The number of valid pixels in the y direction.
     * @param noData The NoData value to use in the output.
     * @param data The DoDData object that contains references to the data sets to use.
     */
double FISRuleSet::calculate(std::vector<double>& dataArrays, bool checkNoData,
                             std::vector<double>& noDataValues, double noData) {

    FISMemberFunction impMf, aggMf;
    FISRule* rule;
    double impValue;
    double v;

    if (checkNoData)
    {

        bool ok = true;
        for (int i=0; i<nInputs_; i++)
        {
            v = dataArrays[i];
            if (v == noDataValues[i])
            {
                ok = false;
                break;
            }
            for (int j=0; j<inputs_[i].n_; j++)
                fuzzyInputs_[i][j] = inputs_[i].mfs_[j].fuzzify(v);
        }
        if (ok)
        {
            for (int r=0; r<nRules_; r++)
            {
                rule = &rules_[r];
                impValue = fuzzyInputs_[rule->inputs_[0]][rule->mfs_[0]];
                for (int m=1; m<rule->n_; m++)
                    impValue = rule->operator_(impValue, fuzzyInputs_[rule->inputs_[m]][rule->mfs_[m]]);
                implicator_(rule->output_, &impMf, impValue, rule->weight_);
                aggregator_(&impMf, &aggMf);
            }

            return defuzzifier_(&aggMf);
        }
        else
        {
            return noData;
        }
    }
    else
    {
        for (int i=0; i<nInputs_; i++)
        {
            v = dataArrays[i];
            for (int j=0; j<inputs_[i].n_; j++)
                fuzzyInputs_[i][j] = inputs_[i].mfs_[j].fuzzify(v);
        }
        for (int r=0; r<nRules_; r++)
        {
            rule = &rules_[r];
            impValue = fuzzyInputs_[rule->inputs_[0]][rule->mfs_[0]];
            for (int m=1; m<rule->n_; m++)
                impValue = rule->operator_(impValue, fuzzyInputs_[rule->inputs_[m]][rule->mfs_[m]]);
            implicator_(rule->output_, &impMf, impValue, rule->weight_);
            aggregator_(&impMf, &aggMf);
        }
        return defuzzifier_(&aggMf);
    }
}

/**
     * Get the name of the input variable at the specified index.
     * Input variables exist in the rule set in the same order that they were added.
     * @param i The input index.
     * @return The name of the input variable at index i.
     */
const char* FISRuleSet::getInputName(int i) {
    for (std::map<std::string, int>::iterator it=indices_.begin(); it!=indices_.end(); it++)
        if (i == (*it).second)
            return (*it).first.c_str();
    return "";
}

/**
     * Get the objects error message.
     * @return The error message.
     */
const char* FISRuleSet::getMsg() {
    const QByteArray qbReturn = msg_.toLocal8Bit();
    return qbReturn.data();
}

/**
     * Get the number of inputs in the rule set.
     * @return The number of input variables.
     */
int FISRuleSet::numInputs() {
    return nInputs_;
}

/**
     * Parse a Matlab Fuzzy Toolbox .fis file for a rule set.
     * @param fn The filename of the file to parse.
     * @return True if successful, false otherwise.
     */
bool FISRuleSet::parseFile(QString fn) {
    bool systemOK = false;
    bool inputOK = false;
    bool outputOK = false;
    bool rulesOK = false;
    const QByteArray qbFn = fn.toLocal8Bit();
    std::ifstream fisFile(qbFn.data());
    if (!fisFile.good())
        return setError(QString("Could not open %1").arg(qbFn.data()));
    std::string line;
    getline(fisFile, line);
    while (!fisFile.eof()) {
        if ("[System]" == line.substr(0, 8)) {
            if (parseSystem(fisFile, line))
                systemOK = true;
            else
                return false;
        }
        if ("[Input" == line.substr(0, 6)) {
            if (parseInput(fisFile, line))
                inputOK = true;
            else
                return false;
            continue;
        }
        if ("[Output" == line.substr(0, 7)) {
            if (parseOutput(fisFile, line))
                outputOK = true;
            else
                return false;
            continue;
        }
        if ("[Rules]" == line.substr(0, 7)) {
            if (parseRules(fisFile, line))
                rulesOK = true;
            else
                return false;
            continue;
        }
        getline(fisFile, line);
    }
    fisFile.close();
    if (!systemOK)
        return setError(QString("No [System] section in %1.").arg(fn));
    else if (!inputOK)
        return setError(QString("No [Input] section in %1.").arg(fn));
    else if (!outputOK)
        return setError(QString("No [Output] section in %1.").arg(fn));
    else if (!rulesOK)
        return setError(QString("No [Rules] section in %1.").arg(fn));
    return valid();
}

/**
     * Add a set of membership functions that make up an input variable.
     * Unlike addInputMFSet, this assumes allows the user to choose the index of the input set, but that
     * index must already exist.
     * @param index The index of the input variable. The FISRuleSet must already know that there are at
     *              least that many variables (it does not automatically increment).
     * @param name The name of the input variable. Cannot contain spaces.
     * @param mfs The membership function set.
     * @return True of the set could be added, false otherwise.
     */
bool FISRuleSet::setInputMFSet(int index, const char* name, FISMemberFunctionSet mfs) {
    if (0 > index || nInputs_ <= index)
        return setError(QString("Invalid index for input: %1").arg(name) );
    else if ((indices_.end() != indices_.find(name)) && (indices_[name] != index))
        return setError(QString("The name '%1' is already in use.").arg(name));
    else if (std::string::npos != std::string(name).find(' '))
        return setError(QString("Invalid name '%1'. Spaces are not allowed.").arg(name));
    else {
        inputs_[index] = mfs;
        indices_[name] = index;
        return true;
    }
}

bool FISRuleSet::valid() {
    if (nInputs_ == 0)
        return setError("No FIS inputs.");
    else if (nRules_ == 0)
        return setError("No FIS rules.");
    for (int i=0; i<nInputs_; i++)
        if (!inputs_[i].valid())
            return setError(QString("Invalid input number: %1.").arg(i));
    return true;
}

/****************************************************************************************************/
/************************************     Standalone Functions     **********************************/
/****************************************************************************************************/

/**
     * Figures out where two lines intersect and if the intersection is in the line segments.
     * @param ax1 The x coordinate for the first end point of segment A.
     * @param ay1 The y coordinate for the first end point of segment A.
     * @param ax2 The x coordinate for the second end point of segment A.
     * @param ay2 The y coordinate for the second end point of segment A.
     * @param bx1 The x coordinate for the first end point of segment B.
     * @param by1 The y coordinate for the first end point of segment B.
     * @param bx2 The x coordinate for the second end point of segment B.
     * @param by2 The y coordinate for the second end point of segment B.
     * @param x The double to put the x coordinate of the intersection point into.
     * @param y The double to put the y coordinate of the intersection point into.
     * @return True if the intersection occurs in segments A and B, false otherwise.
     */
bool FISIntersectLines(double ax1, double ay1, double ax2, double ay2, double bx1, double by1,
                       double bx2, double by2, double* x, double* y) {
    double aSlope = (ay2 - ay1) / (ax2 - ax1);
    double bSlope = (by2 - by1) / (bx2 - bx1);
    double aIntercept = ay1 - aSlope * ax1;
    double bIntercept = by1 - bSlope * bx1;
    if (aSlope == bSlope)
        return false;
    else {
        *x = (bIntercept - aIntercept) / (aSlope - bSlope);
        *y = aSlope * *x + aIntercept;
        if ((ax1 < *x && *x < ax2) && (bx1 < *x && *x < bx2))
            return true;
        else
            return false;
    }
}

/**
     * The Maximum OR fuzzy operator.
     * @param n1 The first value to apply the operator to.
     * @param n2 The second value to apply the operator to.
     * @return The output from the fuzzy operator.
     */
double FISOpMax(double n1, double n2) {
    return std::max(n1, n2);
}

/**
     * The Minimum AND fuzzy operator.
     * @param n1 The first value to apply the operator to.
     * @param n2 The second value to apply the operator to.
     * @return The output from the fuzzy operator.
     */
double FISOpMin(double n1, double n2) {
    return std::min(n1, n2);
}

/**
     * The Probabalistic OR fuzzy operator.
     * @param n1 The first value to apply the operator to.
     * @param n2 The second value to apply the operator to.
     * @return The output from the fuzzy operator.
     */
double FISOpProbor(double n1, double n2) {
    return n1 + n2 - n1 * n2;
}

/**
     * The Product AND fuzzy operator.
     * @param n1 The first value to apply the operator to.
     * @param n2 The second value to apply the operator to.
     * @return The output from the fuzzy operator.
     */
double FISOpProduct(double n1, double n2) {
    return n1 * n2;
}

}
