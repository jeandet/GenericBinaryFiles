#ifndef SEGMENT_H
#define SEGMENT_H
#include <QString>
#include <vector>
#include <stdexcept>
#include <iostream>

class Segment
{
public:
    Segment(){}
    Segment(const char* data, quint64 size, quint64 address, const QString& header=0)
        :p_header(header),p_data(data,data+size),p_address(address)
    {
    }
    Segment(const std::vector<char>& data, quint64 address, const QString& header=0)
        :p_header(header),p_data(data),p_address(address)
    {
    }

    Segment(std::vector<char>&& data, quint64 address, const QString& header=0)
        :p_header(header),p_data(std::move(data)),p_address(address)
    {
    }

    Segment (const Segment &source):
        p_header(source.p_header),p_data(source.p_data),p_address(source.p_address)
    {
    }

    Segment(Segment&& source) noexcept
        :p_header(std::move(source.p_header)),p_data(std::move(source.p_data))
        ,p_address(std::move(source.p_address))
    {
    }

    Segment &operator=(const Segment &copy)=default;
    /**
     * @brief merge If current fragment is next to given one then it merges into current.
     * @param other Fragment to merge.
     * @return True if operation succeed, likely if two fragments are contiguous.
     */
    bool merge(const Segment& other)
    {
        if(this->p_address > other.p_address)
        {
            if(this->p_address == (other.p_address + other.p_data.size()))
            {
                std::vector<char> tmp(this->p_data.size()+other.p_data.size());
                tmp.insert(tmp.end(), other.p_data.begin(), other.p_data.end());
                tmp.insert(tmp.end(), this->p_data.begin(), this->p_data.end());
                this->p_data = std::move(tmp);
                return  true;
            }
        }
        if(other.p_address > this->p_address)
        {
            if(other.p_address == (this->p_address + this->p_data.size()))
            {
                this->p_data.reserve(this->p_data.size()+other.p_data.size());
                this->p_data.insert(this->p_data.end(), other.p_data.begin(), other.p_data.end());
                return  true;
            }
        }
        return  false;
    }
    const QString& header(){return  this->p_header;}
    quint64 size(){return  this->p_data.size();}
    quint64 address(){return  this->p_address;}

    char &operator[](int i){return p_data[i]; }
    const char &operator[](int i) const{return p_data[i];}
private:
    QString p_header;
    std::vector<char> p_data;
    quint64 p_address;
};
#endif // SEGMENT_H
