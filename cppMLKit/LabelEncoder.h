/*
 * LabelEncoder.h
 *
 * Copyright (C) 2019 Jingying Li<jyli1230@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef CPPMLKIT_LABELENCODER_H
#define CPPMLKIT_LABELENCODER_H

#include <unordered_map>
#include <string>

#include <cppMLKit/Matrix.h>

template <typename T>
class LabelEncoder {
    std::unordered_map<T, int> trans;
public:
    LabelEncoder() {}

    void fit(const std::vector<T> &x) {
        trans.clear();

        int counter = 0;
        for (auto &s : x) {
            auto it = trans.find(s);
            if (it == trans.end()) {
                trans[s] = counter;
                ++counter;
            }
        }
    }

    std::vector<int> transform(const std::vector<T> &x, int errorVal = -1) const {
        std::vector<int> ret;

        for (auto &s : x) {
            auto it = trans.find(s);
            if (it != trans.end()) {
                ret.push_back(it->second);
            } else {
                ret.push_back(errorVal);
            }
        }

        return ret;
    }

    std::vector<int> fitTransform(const std::vector<T> &x) {
        trans.clear();

        std::vector<int> ret;
        int counter = 0;

        for (auto &s : x) {
            auto it = trans.find(s);
            if (it != trans.end()) {
                ret.push_back(it->second);
            } else {
                trans[s] = counter;
                ret.push_back(counter);
                ++counter;
            }
        }

        return ret;
    }
};


#endif //CPPMLKIT_LABELENCODER_H
