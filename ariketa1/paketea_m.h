//
// Generated file, do not edit! Created by nedtool 5.6 from paketea.msg.
//

#ifndef __PAKETEA_M_H
#define __PAKETEA_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>paketea.msg:15</tt> by nedtool.
 * <pre>
 * //
 * // This program is free software: you can redistribute it and/or modify
 * // it under the terms of the GNU Lesser General Public License as published by
 * // the Free Software Foundation, either version 3 of the License, or
 * // (at your option) any later version.
 * // 
 * // This program is distributed in the hope that it will be useful,
 * // but WITHOUT ANY WARRANTY; without even the implied warranty of
 * // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * // GNU Lesser General Public License for more details.
 * // 
 * // You should have received a copy of the GNU Lesser General Public License
 * // along with this program.  If not, see http://www.gnu.org/licenses/.
 * //
 * packet paketea
 * {
 *     unsigned int Type;
 * }
 * </pre>
 */
class paketea : public ::omnetpp::cPacket
{
  protected:
    unsigned int Type;

  private:
    void copy(const paketea& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const paketea&);

  public:
    paketea(const char *name=nullptr, short kind=0);
    paketea(const paketea& other);
    virtual ~paketea();
    paketea& operator=(const paketea& other);
    virtual paketea *dup() const override {return new paketea(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getType() const;
    virtual void setType(unsigned int Type);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const paketea& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, paketea& obj) {obj.parsimUnpack(b);}


#endif // ifndef __PAKETEA_M_H
