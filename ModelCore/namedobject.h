#ifndef NAMEDOBJECT_H
#define NAMEDOBJECT_H

#include <string>

namespace HabitatModel {

/**
 * @brief
 *
 */
class NamedObject
{
public:
    /**
     * @brief
     *
     * @param sName
     */
    NamedObject(const char * sName);

    // Copy Constructor
    /**
     * @brief
     *
     * @param obj
     */
    NamedObject(const NamedObject &obj);

    // Assignment Operator
    /**
     * @brief
     *
     * @param obj
     */
    void operator=(const NamedObject &obj);

    /**
     * @brief
     *
     * @return std::string
     */
    std::string GetName() const;
    /**
     * @brief
     *
     * @param sName
     */
    void SetName(const char * sName);

private:
    std::string m_sName; /**< TODO */
};

/**
 * @brief
 *
 */
class NamedObjectWithID : public NamedObject
{
public:
    /**
     * @brief
     *
     * @param sName
     */
    NamedObjectWithID(const char * sName, int nID);

    // Copy Constructor
    /**
     * @brief
     *
     * @param obj
     */
    NamedObjectWithID(const NamedObject &obj);

    // Assignment Operator
    /**
     * @brief
     *
     * @param obj
     */
    void operator=(const NamedObject &obj);

    /**
     * @brief
     *
     * @return std::string
     */
    int GetID() const;
    /**
     * @brief
     *
     * @param sName
     */
    void SetID(int nID);

private:
    int id; /**< TODO */

};
}


#endif // NAMEDOBJECT_H
