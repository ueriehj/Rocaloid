 /*
  * RDLIO.h
  *
  * Copyright (C) 2013 - Rocaloid Development Group(RDG)
  *
  * Created by rgwan 
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program. If not, see <http://www.gnu.org/licenses/>.
  */
 #ifndef RDLIO_H
 #define RDLIO_H
 
 namespace RDLIO
{
	#define Trim(x) trim(x)
	#define SliceTo(x) ltrim(x)
	int TestIfIsInt(string _String);
	int TestIfIsDouble(string _String);
	double TestIfIsDoubleAndPositive(string _String);
	int TestIfIsIntAndPositive(string _String) ;
	double TestIfIsDoubleNotNegative(string _String);
	int TestIfIsIntNotNegative(string _String) ;
	bool TestIfIsBoolean(string _String);
	int TestIfIsPresetedEnvelope(string _String);
}
class RDLReader
{
	public:
		string Read();
	private:
		//Private Reader As StreamReader
		string LineBuffer ;
		int LineBufferQ ;
		int LineBufferPointer; 
		string StrStore;
};
 #endif /*RDLIO _H */
 