#ifndef CELLPVARS_H
#define CELLPVARS_H
#include <map>
#include <random>
#include <array>
#include <memory>
#include <chrono>

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

using namespace std;

class CellPvars {
    public:
        //Types
        enum Distribution {
            none = 0,
            normal = 1,
            lognormal = 2
        };

        struct IonChanParam {
            IonChanParam(Distribution dist = Distribution::none,
                double val1 = 0, double val2 = 0) {
                this->dist = dist;
                this->val[0] = val1;
                this->val[1] = val2;
            }
            IonChanParam(const IonChanParam&) = default;
            virtual ~IonChanParam() {}
            Distribution dist;
            array<double,2> val;
            /*  what these values are depends on dist
             *  for none: val[0] = starting value, val[1] = increment amount
             *  for normal & lognormal: val[0] = mean, val[1] = standard deviation
             */
            virtual string str(string name);
        };

        virtual ~CellPvars();
        typedef std::map<string,IonChanParam*>::const_iterator const_iterator;

        //Functions
        virtual CellPvars* clone() = 0;

        virtual void setIonChanParams() = 0;
        virtual void calcIonChanParams() = 0;

        virtual void writePvars(QXmlStreamWriter& xml) = 0;
        virtual void readPvars(QXmlStreamReader& xml) = 0;

        virtual void insert(string,IonChanParam) = 0;
        virtual void erase(string);
        virtual int size() const;
        virtual void clear();
        virtual IonChanParam* at(string);
        virtual const_iterator begin() const;
        virtual const_iterator end() const;

    protected:
        shared_ptr<default_random_engine> generator =
            make_shared<default_random_engine>(std::chrono::system_clock::now().time_since_epoch().count());
        map<string,IonChanParam*> pvars;
};
#endif
