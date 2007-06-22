/*************************************************************************
Copyright (c) 2007, Regents of the University of Iowa

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of The University of Iowa nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************/
//
// Name: objectmap.cpp
// Author: John Dill
// Date: 5/17/00
//

#include "objectmap.h"

uiig::CObjectMap::CObjectMap( void )
:uiig::CImageFlat<unsigned char>(),Version(VERSION6),
NumberOfObjects(0),NeedsSaving(0), NeedsRegionsCalculated(0)
{
  this->ImageClear();
  {
    for (int i = 0; i < 256; i++)
    {
      ObjectArray[i] = NULL;
      ShowObject[i] = 0;
      MinimumPixelValue[i] = 0;
      MaximumPixelValue[i] = 0;
    }
  }

  // Setting object zero as the background
  ObjectArray[0] = new uiig::CObjectEntry();
  this->getObjectEntry(0).setName("Background");
  this->getObjectEntry(0).setDisplayFlag(0);
  this->getObjectEntry(0).setOpacity(0);
  this->getObjectEntry(0).setOpacityThickness(0);
  this->getObjectEntry(0).setEndRed(0);
  this->getObjectEntry(0).setEndGreen(0);
  this->getObjectEntry(0).setEndBlue(0);
  this->getObjectEntry(0).setShades(1);
}


uiig::CObjectMap::CObjectMap( const int _iX, const int _iY, const int _iZ )
:uiig::CImageFlat<unsigned char>(_iX,_iY,_iZ),Version(VERSION6),
NumberOfObjects(0),NeedsSaving(0), NeedsRegionsCalculated(0)
{
  this->ImageClear();
  {
    for (int i = 0; i < 256; i++)
    {
      ObjectArray[i] = NULL;
      ShowObject[i] = 0;
      MinimumPixelValue[i] = 0;
      MaximumPixelValue[i] = 0;
    }
  }

  // Setting object zero as the background
  ObjectArray[0] = new uiig::CObjectEntry();
  this->getObjectEntry(0).setName("Background");
  this->getObjectEntry(0).setDisplayFlag(0);
  this->getObjectEntry(0).setOpacity(0);
  this->getObjectEntry(0).setOpacityThickness(0);
  this->getObjectEntry(0).setEndRed(0);
  this->getObjectEntry(0).setEndGreen(0);
  this->getObjectEntry(0).setEndBlue(0);
  this->getObjectEntry(0).setShades(1);
}


uiig::CObjectMap::CObjectMap( const uiig::CObjectMap & rhs )
{
  this->CopyBaseImage( rhs );
  Version=rhs.Version;
  NumberOfObjects=rhs.NumberOfObjects;
  NeedsSaving=rhs.NeedsSaving;
  NeedsRegionsCalculated=rhs.NeedsRegionsCalculated;
  for(int i=0; i < 256; i++)
  {
    if(rhs.ObjectArray[i] != NULL)
    {
      ObjectArray[i] = new uiig::CObjectEntry();
      *(ObjectArray[i])=*(rhs.ObjectArray[i]);
    }
    else
    {
      ObjectArray[i] = NULL;
    }
    ShowObject[i]=rhs.ShowObject[i];
    MinimumPixelValue[i]=rhs.MinimumPixelValue[i];
    MaximumPixelValue[i]=rhs.MaximumPixelValue[i];
  }
}


uiig::CObjectMap::~CObjectMap( void )
{
  for(int i=0; i < 256; i++)
  {
    if(ObjectArray[i] != NULL)
    {
      delete ObjectArray[i];
    }
  }
}


bool uiig::CObjectMap::CopyBaseImage( const uiig::CObjectMap& rhs )
{
  // This is the summary of what the CImage's do for their copying
  this->ImageReinitialize(
    rhs.getXDim(), rhs.getXVox(),
    rhs.getYDim(), rhs.getYVox(),
    rhs.getZDim(), rhs.getZVox(),
    rhs.getTDim(), rhs.getTVox());
  ::memcpy( static_cast<void *>( this->getDataPtr() ), static_cast<void *>(rhs.getDataPtr()) ,
    sizeof(unsigned char)*rhs.getTotalVox() );
  // End of hacked copy method
  return true;
}


uiig::CObjectMap &  uiig::CObjectMap::operator=( const uiig::CObjectMap & rhs )
{

  this->CopyBaseImage( rhs );

  Version=rhs.Version;
  NumberOfObjects=rhs.NumberOfObjects;
  NeedsSaving=rhs.NeedsSaving;
  NeedsRegionsCalculated=rhs.NeedsRegionsCalculated;
  for(int i=0; i < 256; i++)
  {
    if(rhs.ObjectArray[i] != NULL)
    {
      ObjectArray[i] = new uiig::CObjectEntry();
      *(ObjectArray[i])=*(rhs.ObjectArray[i]);
    }
    else
    {
      if(ObjectArray[i] != NULL)
      {
        delete ObjectArray[i];
      }
    }
    ShowObject[i]=rhs.ShowObject[i];
    MinimumPixelValue[i]=rhs.MinimumPixelValue[i];
    MaximumPixelValue[i]=rhs.MaximumPixelValue[i];
  }
  return *this;
}


int uiig::CObjectMap::getObjectIndex( const std::string &ObjectName  )
{
  for(int index=0; index<=this->getNumberOfObjects(); index++)
  {
    if(ObjectName == this->getObjectEntry(index).getName() )
    {
      return index;
    }
  }
  //Failure if not found.
  return -1;
}


uiig::CObjectEntry & uiig::CObjectMap::getObjectEntry( const int index )
{
  return *(ObjectArray[index]);
}

const uiig::CObjectEntry & uiig::CObjectMap::getObjectEntry( const int index ) const
{
  return *(ObjectArray[index]);
}


void uiig::CObjectMap::ReinitializeObjectMap(const int _iX, const int _iY, const int _iZ)
{
  this->ImageReinitialize(_iX, _iY, _iZ);
  this->ImageClear();
  {
    for(int i=0; i < 256; i++)
    {
      if(ObjectArray[i] != NULL)
      {
        delete ObjectArray[i];
      }
    }
  }

  {
    for (int i = 0; i < 256; i++)
    {
      ObjectArray[i] = NULL;
      ShowObject[i] = 0;
      MinimumPixelValue[i] = 0;
      MaximumPixelValue[i] = 0;
    }
  }

  Version = VERSION6;
  NumberOfObjects = 0;
  NeedsSaving = 0;
  NeedsRegionsCalculated = 0;

  // Setting object zero as the background
  ObjectArray[0] = new uiig::CObjectEntry();
  this->getObjectEntry(0).setName("Background");
  this->getObjectEntry(0).setDisplayFlag(0);
  this->getObjectEntry(0).setOpacity(0);
  this->getObjectEntry(0).setOpacityThickness(0);
  this->getObjectEntry(0).setEndRed(0);
  this->getObjectEntry(0).setEndGreen(0);
  this->getObjectEntry(0).setEndBlue(0);
  this->getObjectEntry(0).setShades(1);
}


void SwapObjectEndedness(uiig::Object * ObjToChange)
{
  uiig::FileIOUtility util;
  util.FourByteSwap(&(ObjToChange->DisplayFlag));
  util.FourByteSwap(&(ObjToChange->Shades));
  util.FourByteSwap(&(ObjToChange->StartRed));
  util.FourByteSwap(&(ObjToChange->StartGreen));
  util.FourByteSwap(&(ObjToChange->StartBlue));
  util.FourByteSwap(&(ObjToChange->EndRed));
  util.FourByteSwap(&(ObjToChange->EndGreen));
  util.FourByteSwap(&(ObjToChange->EndBlue));
  util.FourByteSwap(&(ObjToChange->XRotation));
  util.FourByteSwap(&(ObjToChange->YRotation));
  util.FourByteSwap(&(ObjToChange->ZRotation));
  util.FourByteSwap(&(ObjToChange->XTranslation));
  util.FourByteSwap(&(ObjToChange->YTranslation));
  util.FourByteSwap(&(ObjToChange->ZTranslation));
  util.FourByteSwap(&(ObjToChange->XCenter));
  util.FourByteSwap(&(ObjToChange->YCenter));
  util.FourByteSwap(&(ObjToChange->ZCenter));
  util.FourByteSwap(&(ObjToChange->XRotationIncrement));
  util.FourByteSwap(&(ObjToChange->YRotationIncrement));
  util.FourByteSwap(&(ObjToChange->ZRotationIncrement));
  util.FourByteSwap(&(ObjToChange->XTranslationIncrement));
  util.FourByteSwap(&(ObjToChange->YTranslationIncrement));
  util.FourByteSwap(&(ObjToChange->ZTranslationIncrement));
  util.TwoByteSwap(&(ObjToChange->MinimumXValue));
  util.TwoByteSwap(&(ObjToChange->MinimumYValue));
  util.TwoByteSwap(&(ObjToChange->MinimumZValue));
  util.TwoByteSwap(&(ObjToChange->MaximumXValue));
  util.TwoByteSwap(&(ObjToChange->MaximumYValue));
  util.TwoByteSwap(&(ObjToChange->MaximumZValue));
  util.FourByteSwap(&(ObjToChange->Opacity));
  util.FourByteSwap(&(ObjToChange->OpacityThickness));
  util.FourByteSwap(&(ObjToChange->Dummy));
}


bool uiig::CObjectMap::ReadObjectFile( const std::string& filename )
{
  int header[5];
  int i, j, index;
  FILE *fptr;
  unsigned char buffer[BUFFERSIZE];

  // Opening the file
  if ( ( fptr = ::fopen( filename.c_str(), "rb" ) ) == NULL)
  {
    ::fprintf( stderr, "Error: Could not open %s\n", filename.c_str());
    exit(-1);
  }

  // Reading the header, which contains the version number, the size, and the
  // number of objects
  if ( ::fread( header, sizeof(int), 5, fptr) != 5 )
  {
    ::fprintf( stderr, "Error: Could not read header of %s\n", filename.c_str());
    exit(-1);
  }

  bool NeedByteSwap=false;
  //Do byte swapping if necessary.
  uiig::FileIOUtility util;
  if(header[0] == 1323699456 || header[0] == -1913442047)    // Byte swapping needed (Number is byte swapped number of VERSION6 or VERSION8 )
  {
    NeedByteSwap=true;
    util.FourByteSwap(&(header[0]));
    util.FourByteSwap(&(header[1]));
    util.FourByteSwap(&(header[2]));
    util.FourByteSwap(&(header[3]));
    util.FourByteSwap(&(header[4]));
  }

  // Reading the Header into the class
  this->Version = header[0];
  const int XSize = header[1];
  const int YSize = header[2];
  const int ZSize = header[3];
  this->NumberOfObjects = header[4];
  const int VolumeSize=XSize*YSize*ZSize;
  printf("TEST. Obj headers: NumberOfObjects %d \n", header[4]);
  // Validating the version number
  if (Version != VERSION6 && Version != VERSION8)
  {
    ::fprintf( stderr, "Error: Can only process version 6 and version 8 analyze object files.\n" );
    ::fclose( fptr );
    return false;
  }

  // In version 8, the header file has a new field after number of objects, before name,
  // which is nvols, with type int. This field allows 4D object maps. 
  // Further updating of objectmap related programs are to be developed to 
  // obtain, utilize this field. Xiujuan Geng May 04, 2007
  if( Version == VERSION8 )
  {
    int nvols[1];
    if ( ::fread( nvols, sizeof(int), 1, fptr) != 1 )
    {
      ::fprintf( stderr, "Error: Could not read header of %s\n", filename.c_str());
      exit(-1);
    }
  }

  // Error checking the number of objects in the object file
  if ((NumberOfObjects < 1) || (NumberOfObjects > 255))
  {
    ::fprintf( stderr, "Error: Invalid number of object files.\n" );
    ::fclose( fptr );
    return false;
  }

  // Read in the first header to determine whether it is a background defined
  // Analyze object file or not
  ObjectArray[1] = new uiig::CObjectEntry();

  if (ObjectArray[1] == NULL)
  {
    ::fprintf(stderr, "Unable to allocate memory for object #1.\n");
    exit(-1);
  }

  // Reading the object directly into the ObjectEntry variable
  if (::fread((ObjectArray[1]->getObjectPointer()), sizeof(Object), 1, fptr) != 1)
  {
    ::fprintf(stderr, "6: Unable to read in object #1 description of %s\n", filename.c_str());
    exit(-1);
  }
  if(NeedByteSwap)
  {
    SwapObjectEndedness(ObjectArray[1]->getObjectPointer());
  }

  bool backgroundDefinedObjectMap;

  if (::strcmp(getObjectEntry(1).getName().c_str(),"Background") == 0)
    backgroundDefinedObjectMap = true;
  else
    backgroundDefinedObjectMap = false;
  
  //Added temporarilly May 05, 2007
  backgroundDefinedObjectMap=true;

  if (backgroundDefinedObjectMap == true)
  {
    // The background is already defined, so start with the index for 1
    delete ObjectArray[1];
    ObjectArray[1] = NULL;

    for (i = 1; i < NumberOfObjects; i++)
    {
      // Allocating a object to be created
      ObjectArray[i] = new uiig::CObjectEntry();

      // Checking to see if the memory was available
      if (ObjectArray[i] == NULL)
      {
        ::fprintf(stderr, "5: Unable to allocate object #%d description of %s\n", i, filename.c_str());
        exit(-1);
      }

      // Reading the object directly into the ObjectEntry variable
      if (::fread((ObjectArray[i]->getObjectPointer()), sizeof(Object), 1, fptr) != 1)
      {
        ::fprintf(stderr, "6: Unable to read in object #%d description of %s\n", i, filename.c_str());
        exit(-1);
      }
      if(NeedByteSwap)
      {
        SwapObjectEndedness(ObjectArray[i]->getObjectPointer());
      }
    }

    // Since one of the objects read in was declared to be background, take away one object
    NumberOfObjects = NumberOfObjects - 1;
  }
  else
  {
    // Zero is already declared background and 1 is not background, so read in everything else
    for (i = 1; i < NumberOfObjects; i++)
    {
      // Allocating a object to be created
      ObjectArray[i+1] = new uiig::CObjectEntry();

      // Checking to see if the memory was available
      if (ObjectArray[i+1] == NULL)
      {
        ::fprintf(stderr, "5: Unable to allocate object #%d description of %s\n", i, filename.c_str());
        exit(-1);
      }

      // Reading the object directly into the ObjectEntry variable
      if (::fread((ObjectArray[i+1]->getObjectPointer()), sizeof(Object), 1, fptr) != 1)
      {
        ::fprintf(stderr, "6: Unable to read in object #%d description of %s\n", i, filename.c_str());
        exit(-1);
      }
      if(NeedByteSwap)
      {
        SwapObjectEndedness(ObjectArray[i+1]->getObjectPointer());
      }
    }
  }

  // Creating the image volume
  this->ImageReinitialize(XSize, YSize, ZSize, 1);

  // Decoding the run length encoded raw data into an unsigned char volume
  index = 0;

  // The file consists of unsigned character pairs which represents the encoding of the data
  // The character pairs have the form of length, tag value.  Note also that the data in
  // Analyze object files are run length encoded a plane at a time.

  if (backgroundDefinedObjectMap == true)
  {
    while (::fread(buffer,1,BUFFERSIZE,fptr) > 0)
    {
      for (i = 0; i < BUFFERSIZE; i+=2)
      {
        for (j = 0; j < buffer[i]; j++)
        {
          this->operator()(index) = buffer[i+1];
    index++;
        }

        if (index >= VolumeSize)
          break;
      }
    }
  }
  else
  {
    while (::fread(buffer,1,BUFFERSIZE,fptr) > 0)
    {
      for (i = 0; i < BUFFERSIZE; i+=2)
      {
        for (j = 0; j < buffer[i]; j++)
        {
          // Copy this information to the index above background (which is at index 0)
          this->operator()(index) = buffer[i+1] + 1;
    index++;
        }
        if (index >= VolumeSize)
          break;
      }
    }
  }

  if (index != VolumeSize)
  {
    ::fprintf(stderr, "Warning: Error decoding run-length encoding.  \n");
    if(index < VolumeSize)
    {
      ::fprintf(stderr, "Warning: file underrun.  \n");
    }
    else
    {
      ::fprintf(stderr, "Warning: file overrun.  \n");
    }
    return false;
  }

  return true;
}


bool uiig::CObjectMap::WriteObjectFile( const std::string& filename )
{

  FILE *fptr;
  std::string tempfilename=filename;

  // This assumes that the final '.' in a file name is the delimiter
  // for the file's extension type
  const std::string::size_type it = filename.find_last_of( "." );

  //Note: if there is no extension, then it=basic_string::npos;
  if (it > filename.length())
  {
    tempfilename+=".obj";
  }
  else
  {
    // Now we have the index of the extension, make a new string
    // that extends from the first letter of the extension to the end of filename
    std::string fileExt( filename, it, filename.length() );
    if (fileExt != ".obj")
    {
      tempfilename+=".obj";
    }
  }
  // Opening the file
  if ( ( fptr = ::fopen( tempfilename.c_str(), "wb" ) ) == NULL)
  {
    ::fprintf( stderr, "Error: Could not open %s\n", filename.c_str());
    exit(-1);
  }

  int header[5];

  // Getting the Header of the class
  //header[0]=VERSION6;
  printf("Version is %d \n", Version);
  header[0]=Version;
  header[1]=this->getXDim();
  header[2]=this->getYDim();
  header[3]=this->getZDim();
                                 // Include the background object when writing the .obj file
  header[4]=this->NumberOfObjects + 1;

  bool NeedByteSwap = false;

  // Byte swapping needed (Analyze files are always written in BIG ENDIAN
  uiig::FileIOUtility util;
  if(util.getMachineEndianess() == uiig::FileIOUtility::DT_LITTLE_ENDIAN)
  {
    NeedByteSwap=true;
    util.FourByteSwap(&(header[0]));
    util.FourByteSwap(&(header[1]));
    util.FourByteSwap(&(header[2]));
    util.FourByteSwap(&(header[3]));
    util.FourByteSwap(&(header[4]));
  }

  // Writing the header, which contains the version number, the size, and the
  // number of objects
  if ( ::fwrite( header, sizeof(int), 5, fptr) != 5 )
  {
    ::fprintf( stderr, "Error: Could not write header of %s\n", filename.c_str());
    exit(-1);
  }

  if( Version == VERSION8 )
  {
    // Set number of volumes to be 1. Need be changed later on. Xiujuan Geng, May 04, 2007
    int nvols[1];    
    nvols[0] = 1; 
    if( NeedByteSwap )
    {
      util.FourByteSwap(&(nvols[0]));
    }
    if ( ::fwrite( nvols, sizeof(int), 1, fptr) != 1 )
    {
      ::fprintf( stderr, "Error: Could not write header of %s\n", filename.c_str());
      exit(-1);
    }    
  }

  // Error checking the number of objects in the object file
  if ((NumberOfObjects < 0) || (NumberOfObjects > 255))
  {
    ::fprintf( stderr, "Error: Invalid number of object files.\n" );
    ::fclose( fptr );
    return false;
  }

  // Since the NumberOfObjects does not reflect the background, the background will be included
  for (int i = 0; i < NumberOfObjects + 1; i++)
  {
    // Using a temporary so that the object file is always written in BIG_ENDIAN mode but does
    // not affect the current object itself
    uiig::CObjectEntry ObjectWrite = this->getObjectEntry(i);

    if (NeedByteSwap == true)
    {
      SwapObjectEndedness(ObjectWrite.getObjectPointer());
    }

    // Writing the ObjectEntry structures
    if (::fwrite(ObjectWrite.getObjectPointer(), sizeof(Object), 1, fptr) != 1)
    {
      ::fprintf(stderr, "13: Unable to write in object #%d description of %s\n", i, filename.c_str());
      exit(-1);
    }
  }

  RunLengthEncodeImage(*this, fptr);

  ::fclose(fptr);
  return true;
}


bool uiig::CObjectMap::AddObjectInRange(const uiig::CImage<float> & InputImage,
const float MinRange, const float MaxRange,
std::string ObjectName, const int EndRed,
const int EndGreen, const int EndBlue,
const int Shades, bool OverWriteObjectFlag)
{
  const int nx=InputImage.getXDim();
  const int ny=InputImage.getYDim();
  const int nz=InputImage.getZDim();
  if(nx != this->getXDim() ||
    ny != this->getYDim() ||
    nz != this->getZDim())
  {
    return false;
  }
  this->NumberOfObjects++;
  this->ObjectArray[NumberOfObjects]=new uiig::CObjectEntry;
  ObjectArray[NumberOfObjects]->setName(ObjectName);
  ObjectArray[NumberOfObjects]->setEndColor(EndRed,EndGreen,EndBlue);
  ObjectArray[NumberOfObjects]->setStartColor(static_cast<int>(0.1F*EndRed),
    static_cast<int>(0.1F*EndGreen),
    static_cast<int>(0.1F*EndBlue));
  ObjectArray[NumberOfObjects]->setShades(Shades);

  for(int k=0; k<nz; k++)
  {
    for( int j=0; j< ny ; j++)
    {
      for( int i=0; i<nx; i++)
      {
        const float CurrentValue=InputImage(i,j,k);

        if(CurrentValue>=MinRange && CurrentValue<=MaxRange)
        {
          // Check to see if there is another object here (not background)
          if (this->Pixel(i,j,k) > 0)
          {
            // This pixel has already been assigned an object
            if (OverWriteObjectFlag == true)
              this->Pixel(i,j,k)=NumberOfObjects;
            // else do nothing
          }
          else
          {
            // Must be background
            this->Pixel(i,j,k)=NumberOfObjects;
          }
        }
      }
    }
  }
  return true;
}


bool uiig::CObjectMap::AddObjectInRange(const uiig::CImage<unsigned char> & InputImage,
const int MinRange, const int MaxRange,
std::string ObjectName, const int EndRed, const int EndGreen,
const int EndBlue, const int Shades, bool OverWriteObjectFlag)
{
  const int nx=InputImage.getXDim();
  const int ny=InputImage.getYDim();
  const int nz=InputImage.getZDim();
  if(nx != this->getXDim() ||
    ny != this->getYDim() ||
    nz != this->getZDim())
  {
    return false;
  }
  this->NumberOfObjects++;
  this->ObjectArray[NumberOfObjects]=new uiig::CObjectEntry;
  ObjectArray[NumberOfObjects]->setName(ObjectName);
  ObjectArray[NumberOfObjects]->setEndColor(EndRed,EndGreen,EndBlue);
  ObjectArray[NumberOfObjects]->setStartColor(static_cast<int>(0.1F*EndRed),
    static_cast<int>(0.1F*EndGreen),
    static_cast<int>(0.1F*EndBlue));
  ObjectArray[NumberOfObjects]->setShades(Shades);

  for(int k=0; k<nz; k++)
  {
    for( int j=0; j<ny ; j++)
    {
      for( int i=0; i<nx; i++)
      {
        const int CurrentValue=InputImage(i,j,k);
        if(CurrentValue>=MinRange && CurrentValue<=MaxRange)
        {
          if (this->Pixel(i,j,k) > 0)
          {
            // This pixel has already been assigned an object
            if (OverWriteObjectFlag == true)
              this->Pixel(i,j,k)=NumberOfObjects;
            // else do nothing
          }
          else
          {
            // Must be background
            this->Pixel(i,j,k)=NumberOfObjects;
          }
        }
      }
    }
  }
  return true;
}


bool uiig::CObjectMap::RemoveObjectByName(const std::string & ObjectName)
{
  if (ObjectName == std::string("Background"))
  {
    // Can not delete background object
    ::fprintf(stderr, "Can not remove the background object.\n");
    return false;
  }

  int objectTag = 0;
  {
    for (int i = 1; i <= this->getNumberOfObjects(); i++)
    {
      if (ObjectName == this->getObjectEntry(i).getName())
      {
        objectTag = i;
        break;
      }
    }
  }

  if (objectTag == 0)
  {
    // Object name not found
    printf("Object name %s was not found.\n", ObjectName.c_str());
    return false;
  }

  // Go through all the object headers and shift them down
  {
    for (int j = objectTag; j < this->getNumberOfObjects(); j++)
    {
      this->getObjectEntry(j) = this->getObjectEntry(j+1);
    }
  }

  // Deleting the last extra data structure
  delete ObjectArray[this->getNumberOfObjects()];
  ObjectArray[this->getNumberOfObjects()] = NULL;

  // Changing the image object identifiers
  {
    for (int i = 0; i < this->getXDim()*this->getYDim()*this->getZDim(); i++)
    {
      if (this->ConstPixel(i) == objectTag)
      {
        // Assign to the background
        this->Pixel(i) = 0;
      }
      else if (this->ConstPixel(i) > objectTag)
      {
        this->Pixel(i) = this->Pixel(i) - 1;
      }
    }
  }

  NumberOfObjects = NumberOfObjects - 1;

  return true;
}


bool uiig::CObjectMap::RemoveObjectByRange(const unsigned char MinRange, const unsigned char MaxRange)
{
  int startObjectTag = MinRange;
  int endObjectTag = MaxRange;

  // Validating ranges
  if (MinRange == 0)
  {
    // Do not delete background object, but continue with the rest of the objects in range
    printf("Can not delete the background object.\n");
    startObjectTag = 1;
  }

  if (MaxRange > this->getNumberOfObjects())
  {
    // Set the end to the number of objects
    printf("Max Range too large.  Setting it too maximum number of objects in the objectmap.\n");
    endObjectTag = this->getNumberOfObjects();
  }

  if (MinRange > MaxRange)
  {
    // Parameters are backwards
    ::fprintf(stderr, "Minimum Range %d and Maximum Range %d for removing objects are invalid.\n",
      MinRange, MaxRange);
    return false;
  }

  int NumberToDelete = endObjectTag - startObjectTag + 1;

  // Go through all the object headers and shift them down
  {
    for (int j = endObjectTag + 1; j <= this->getNumberOfObjects(); j++)
    {
      this->getObjectEntry(j-NumberToDelete) = this->getObjectEntry(j);
    }
  }

  // Deleting the last extra data structure
  {
    for (int i = this->getNumberOfObjects(); i > this->getNumberOfObjects() - NumberToDelete; i--)
    {
      delete ObjectArray[i];
      ObjectArray[i] = NULL;
    }
  }

  // Changing the image object identifiers
  {
    for (int i = 0; i < this->getXDim()*this->getYDim()*this->getZDim(); i++)
    {
      if ((this->ConstPixel(i) >= startObjectTag) && (this->ConstPixel(i) <= endObjectTag))
      {
        // Assign to the background
        this->Pixel(i) = 0;
      }
      else if (this->ConstPixel(i) > endObjectTag)
      {
        this->Pixel(i) = this->Pixel(i) - NumberToDelete;
      }
    }
  }

  NumberOfObjects = NumberOfObjects - NumberToDelete;

  return true;
}


bool uiig::CObjectMap::CheckObjectOverlap( const uiig::CImage<float> & InputImage, const float MinRange, const float MaxRange, const int MinObjectRange, const int MaxObjectRange )
{
  const int nx=InputImage.getXDim();
  const int ny=InputImage.getYDim();
  const int nz=InputImage.getZDim();
  if(nx != this->getXDim() ||
    ny != this->getYDim() ||
    nz != this->getZDim())
  {
    return false;
  }

  // Error checking
  if ( MinObjectRange < 0 )
  {
    ::fprintf( stderr, "Error. Can not have a MinObjectRange < 0!\n" );
    exit(-1);
  }
  if ( MaxObjectRange < 0 )
  {
    ::fprintf( stderr, "Error. Can not have a MaxObjectRange < 0!\n" );
    exit(-1);
  }
  if ( MaxObjectRange < MinObjectRange )
  {
    fprintf( stderr, "Error. MaxObjectRange can not be less than MinObjectRange!\n" );
    exit(-1);
  }

  for(int k=0; k<nz; k++)
  {
    for( int j=0; j<ny ; j++)
    {
      for( int i=0; i<nx; i++)
      {
        const float CurrentValue=InputImage(i,j,k);
        if(CurrentValue>=MinRange && CurrentValue<=MaxRange)
        {
          if (this->Pixel(i,j,k) >= MinObjectRange && this->Pixel(i,j,k) <= MaxObjectRange)
          {
            // This pixel has already been assigned an object in the defined range, overlap occuring
            return true;
          }
        }
      }
    }
  }
  return false;
}


bool uiig::CObjectMap::CheckObjectOverlap( const uiig::CImage<unsigned char> & InputImage, const int MinRange, const int MaxRange, const int MinObjectRange, const int MaxObjectRange )
{
  const int nx=InputImage.getXDim();
  const int ny=InputImage.getYDim();
  const int nz=InputImage.getZDim();
  if(nx != this->getXDim() ||
    ny != this->getYDim() ||
    nz != this->getZDim())
  {
    return false;
  }

  // Error checking
  if ( MinObjectRange < 0 )
  {
    ::fprintf( stderr, "Error. Can not have a MinObjectRange < 0!\n" );
    exit(-1);
  }
  if ( MaxObjectRange < 0 )
  {
    ::fprintf( stderr, "Error. Can not have a MaxObjectRange < 0!\n" );
    exit(-1);
  }
  if ( MaxObjectRange < MinObjectRange )
  {
    fprintf( stderr, "Error. MaxObjectRange can not be less than MinObjectRange!\n" );
    exit(-1);
  }

  for(int k=0; k<nz; k++)
  {
    for( int j=0; j<ny ; j++)
    {
      for( int i=0; i<nx; i++)
      {
        const int CurrentValue=InputImage(i,j,k);
        if(CurrentValue>=MinRange && CurrentValue<=MaxRange)
        {
          if (this->Pixel(i,j,k) >= MinObjectRange && this->Pixel(i,j,k) <= MaxObjectRange)
          {
            // This pixel has already been assigned an object in the defined range, overlap occuring
            return true;
          }
        }
      }
    }
  }
  return false;
}


bool uiig::CObjectMap::WriteObjectByName(const std::string & ObjectName, const std::string & filename)
{
  // Finding the object index with the name indicated
  if (ObjectName == std::string("Background"))
  {
    // Can not write a background object
    ::fprintf(stderr, "Error: Can not write out a background object.\n");
    return false;
  }

  int objectTag = 0;
  {
    for (int i = 1; i <= this->getNumberOfObjects(); i++)
    {
      if (ObjectName == this->getObjectEntry(i).getName())
      {
        objectTag = i;
        break;
      }
    }
  }

  if (objectTag == 0)
  {
    ::fprintf(stderr, "Error.  Object name not found in current objectmap.\n");
    return false;
  }

  FILE *fptr;
  std::string tempfilename=filename;

  // This assumes that the final '.' in a file name is the delimiter
  // for the file's extension type
  const std::string::size_type it = filename.find_last_of( "." );

  //Note: if there is no extension, then it=basic_string::npos;
  if(it > filename.length())
  {
    tempfilename+=".obj";
  }
  else
  {
    // Now we have the index of the extension, make a new string
    // that extends from the first letter of the extension to the end of filename
    std::string fileExt( filename, it, filename.length() );
    if(fileExt != ".obj")
    {
      tempfilename+=".obj";
    }
  }

  // Opening the file
  if ( ( fptr = ::fopen( tempfilename.c_str(), "wb" ) ) == NULL)
  {
    ::fprintf( stderr, "Error: Could not open %s\n", filename.c_str() );
    exit(-1);
  }

  int header[5];

  // Reading the Header into the class
  header[0]=VERSION6;
  header[1]=this->getXDim();
  header[2]=this->getYDim();
  header[3]=this->getZDim();
  header[4]=2;                   // Including the background object when writing the .obj file

  bool NeedByteSwap = false;

  uiig::FileIOUtility util;
  // Byte swapping needed, Analyze object files are always written in BIG ENDIAN format
  if(util.getMachineEndianess() == util.DT_LITTLE_ENDIAN)
  {
    NeedByteSwap=true;
    util.FourByteSwap(&(header[0]));
    util.FourByteSwap(&(header[1]));
    util.FourByteSwap(&(header[2]));
    util.FourByteSwap(&(header[3]));
    util.FourByteSwap(&(header[4]));
  }

  // Creating the binary mask
  uiig::CImageFlat<unsigned char> binaryMask(this->getXDim(), this->getYDim(), this->getZDim());
  binaryMask.ImageClear();

  // Creating the binary mask
  {
    for (int i = 0; i < this->getXDim()*this->getYDim()*this->getZDim(); i++)
    {
      if (this->ConstPixel(i) == objectTag)
      {
        binaryMask.Pixel(i) = 1;
      }
    }
  }

  // Writing the header, which contains the version number, the size, and the
  // number of objects
  if ( ::fwrite( header, sizeof(int), 5, fptr) != 5 )
  {
    ::fprintf( stderr, "Error: Could not write header of %s\n", filename.c_str());
    exit(-1);
  }

  // Since the NumberOfObjects does not reflect the background, the background will be written out

  // Writing the background header
  // Using a temporary so that the object file is always written in BIG_ENDIAN mode but does
  // not affect the current object itself
  uiig::CObjectEntry ObjectWrite = this->getObjectEntry(0);

  if (NeedByteSwap == true)
  {
    SwapObjectEndedness(ObjectWrite.getObjectPointer());
  }

  if (::fwrite((ObjectWrite.getObjectPointer()), sizeof(Object), 1, fptr) != 1)
  {
    ::fprintf(stderr, "13: Unable to write object background of %s\n", filename.c_str());
    exit(-1);
  }

  ObjectWrite = this->getObjectEntry(objectTag);

  if (NeedByteSwap == true)
  {
    SwapObjectEndedness(ObjectWrite.getObjectPointer());
  }

  // Writing the object header
  if (::fwrite((ObjectWrite.getObjectPointer()), sizeof(Object), 1, fptr) != 1)
  {
    ::fprintf(stderr, "13: Unable to write in object #%d description of %s\n", objectTag, filename.c_str());
    exit(-1);
  }

  RunLengthEncodeImage(binaryMask, fptr);

  ::fclose(fptr);
  return true;
}


bool uiig::CObjectMap::WriteDisplayedObjects(const std::string & filenamebase)
{
  FILE *fptr;
  std::string tempfilename=filenamebase;

  for (int objectTag = 1; objectTag <= this->getNumberOfObjects(); objectTag++)
  {

    // DEBUG
    // Look at the case when the user puts in a file with an .obj extension.  Append the
    // number in the middle of the filename, not at the end

    if (getObjectEntry(objectTag).getDisplayFlag() != 0)
    {
      // Append the object number to the filename
      std::string tempfilenumbername = tempfilename + std::string("_");

      std::string filenumber("#");
      filenumber[0] = (char)((int)'0' + objectTag);
      tempfilenumbername += filenumber;

      tempfilenumbername+=".obj";

      // Opening the file
      if ( ( fptr = ::fopen( tempfilenumbername.c_str(), "wb" ) ) == NULL)
      {
        ::fprintf( stderr, "Error: Could not open %s\n", tempfilenumbername.c_str());
        exit(-1);
      }

      int header[5];

      // Reading the Header into the class
      header[0]=VERSION6;
      header[1]=this->getXDim();
      header[2]=this->getYDim();
      header[3]=this->getZDim();
      header[4]=2;               // Including the background object when writing the .obj file

      bool NeedByteSwap = false;

      uiig::FileIOUtility util;
      // Byte swapping needed, Analyze files are always written in BIG ENDIAN format
      if(util.getMachineEndianess() == util.DT_LITTLE_ENDIAN)
      {
        NeedByteSwap=true;
        util.FourByteSwap(&(header[0]));
        util.FourByteSwap(&(header[1]));
        util.FourByteSwap(&(header[2]));
        util.FourByteSwap(&(header[3]));
        util.FourByteSwap(&(header[4]));
      }

      // Creating the binary mask
      uiig::CImageFlat<unsigned char> binaryMask(this->getXDim(), this->getYDim(), this->getZDim());
      binaryMask.ImageClear();

      // Creating the binary mask
      for (int i = 0; i < this->getXDim()*this->getYDim()*this->getZDim(); i++)
      {
        if (this->ConstPixel(i) == objectTag)
        {
          binaryMask.Pixel(i) = 1;
        }
      }

      // Writing the header, which contains the version number, the size, and the
      // number of objects
      if ( ::fwrite( header, sizeof(int), 5, fptr) != 5 )
      {
        ::fprintf( stderr, "Error: Could not write header of %s\n", tempfilenumbername.c_str());
        exit(-1);
      }

      // Since the NumberOfObjects does not reflect the background, the background will be written out

      // Writing the background header
      // Using a temporary so that the object file is always written in BIG_ENDIAN mode but does
      // not affect the current object itself
      uiig::CObjectEntry ObjectWrite = this->getObjectEntry(0);

      if (NeedByteSwap == true)
      {
        SwapObjectEndedness(ObjectWrite.getObjectPointer());
      }

      if (::fwrite((ObjectWrite.getObjectPointer()), sizeof(Object), 1, fptr) != 1)
      {
        ::fprintf(stderr, "13: Unable to write object background of %s\n", tempfilenumbername.c_str());
        exit(-1);
      }

      ObjectWrite = this->getObjectEntry(objectTag);

      if (NeedByteSwap == true)
      {
        SwapObjectEndedness(ObjectWrite.getObjectPointer());
      }

      // Writing the object header
      if (::fwrite((ObjectWrite.getObjectPointer()), sizeof(Object), 1, fptr) != 1)
      {
        ::fprintf(stderr, "13: Unable to write in object #%d description of %s\n",
          objectTag, tempfilenumbername.c_str());
        exit(-1);
      }

      RunLengthEncodeImage(binaryMask, fptr);

      fclose(fptr);
    }
  }
  return true;
}


bool uiig::CObjectMap::CalculateBoundingRegionAndCenter( void )
{
  for (int i = 0; i < NumberOfObjects; i++)
  {
    ObjectArray[i]->setMinimumXValue(this->getXDim());
    ObjectArray[i]->setMaximumXValue(0);
    ObjectArray[i]->setMinimumYValue(this->getYDim());
    ObjectArray[i]->setMaximumYValue(0);
    ObjectArray[i]->setMinimumZValue(this->getZDim());
    ObjectArray[i]->setMaximumZValue(0);
  }

  const int PlaneSize = this->getXDim()*this->getYDim();

  std::vector<long int> xsum(this->getNumberOfObjects());
  std::vector<long int> ysum(this->getNumberOfObjects());
  std::vector<long int> zsum(this->getNumberOfObjects());
  std::vector<long int> xnum(this->getNumberOfObjects());
  std::vector<long int> ynum(this->getNumberOfObjects());
  std::vector<long int> znum(this->getNumberOfObjects());

  {
    for (int i = 0; i < this->getNumberOfObjects(); i++)
    {
      xsum[i] = 0;
      ysum[i] = 0;
      zsum[i] = 0;
      xnum[i] = 0;
      ynum[i] = 0;
      znum[i] = 0;
    }
  }

  int xIndex, yIndex, zIndex, r, s, t;

  for (zIndex=0, t=0; zIndex < this->getZDim(); zIndex++, t+=PlaneSize)
  {
    for (yIndex=0, s=t; yIndex < this->getYDim(); yIndex++, s+=this->getXDim())
    {
      for (xIndex=0, r=s; xIndex < this->getXDim(); xIndex++, r++)
      {
        int object = this->ConstPixel(r);
        if (object > this->getNumberOfObjects())
        {
          ::fprintf(stderr, "Error: There are greater index than number of objects.\n");
          exit(-1);
        }
        if (xIndex < getObjectEntry(object).getMinimumXValue())
          getObjectEntry(object).setMinimumXValue(xIndex);
        if (xIndex > getObjectEntry(object).getMaximumXValue())
          getObjectEntry(object).setMaximumXValue(xIndex);

        if (yIndex < getObjectEntry(object).getMinimumYValue())
          getObjectEntry(object).setMinimumYValue(yIndex);
        if (yIndex > getObjectEntry(object).getMaximumYValue())
          getObjectEntry(object).setMaximumYValue(yIndex);

        if (zIndex < getObjectEntry(object).getMinimumZValue())
          getObjectEntry(object).setMinimumZValue(zIndex);
        if (zIndex > getObjectEntry(object).getMaximumZValue())
          getObjectEntry(object).setMaximumZValue(zIndex);

        xsum[object]+=xIndex;
        ysum[object]+=yIndex;
        zsum[object]+=zIndex;
        xnum[object]++;
        ynum[object]++;
        znum[object]++;
      }
    }
  }

  {
    for (int i = 0; i < this->getNumberOfObjects(); i++)
    {
      if (xnum[i] == 0)
        getObjectEntry(i).setXCenter(0);
      else
        getObjectEntry(i).setXCenter((short int)(xsum[i]/xnum[i] - this->getXDim()/2));

      if (ynum[i] == 0)
        getObjectEntry(i).setYCenter(0);
      else
        getObjectEntry(i).setYCenter((short int)(ysum[i]/ynum[i] - this->getYDim()/2));

      if (znum[i] == 0)
        getObjectEntry(i).setZCenter(0);
      else
        getObjectEntry(i).setZCenter((short int)(zsum[i]/znum[i] - this->getZDim()/2));
    }
  }

  return true;
}


int uiig::CObjectMap::EvenlyShade( void )
{
  int NumberOfShades = 255 / this->getNumberOfObjects();

  // Not allocating shades to the background object
  for (int i = 0; i < this->getNumberOfObjects(); i++)
    getObjectEntry(i+1).setShades(NumberOfShades);

  return NumberOfShades;
}


void uiig::CObjectMap::ConstShade( void )
{
  // Not allocating shades to the background object, and setting each shade to 1
  for (int i = 0; i < this->getNumberOfObjects(); i++)
    getObjectEntry(i+1).setShades( 1 );
}


int uiig::CObjectMap::getVersion( void ) const
{
  return Version;
}


int uiig::CObjectMap::getNumberOfObjects( void ) const
{
  return this->NumberOfObjects;
}


unsigned char uiig::CObjectMap::isObjectShown( const unsigned char index ) const
{
  return ShowObject[index];
}


unsigned char uiig::CObjectMap::getMinimumPixelValue( const unsigned char index ) const
{
  return MinimumPixelValue[index];
}


unsigned char uiig::CObjectMap::getMaximumPixelValue( const unsigned char index ) const
{
  return MaximumPixelValue[index];
}

void uiig::CObjectMap::setMinimumPixelValue( const unsigned char index, const unsigned char value )
{
  MinimumPixelValue[index] = value;
}

void uiig::CObjectMap::setMaximumPixelValue( const unsigned char index, const unsigned char value )
{
  MaximumPixelValue[index] = value;
}

bool RunLengthEncodeImage(const uiig::CImageFlat<unsigned char> & SourceImage, FILE *fptr)
{
  if (fptr == NULL)
  {
    fprintf(stderr, "Error: Null file pointer for runlength encoding data.\n");
    return false;
  }

  // Encoding the run length encoded raw data into an unsigned char volume
  const int VolumeSize=SourceImage.getXDim()*SourceImage.getYDim()*SourceImage.getZDim();
  const int PlaneSize = SourceImage.getXDim()*SourceImage.getYDim();
  int bufferindex=0;
  int planeindex=0;
  int runlength=0;
  int CurrentObjIndex=0;
  const  int buffer_size=16384;
                                 //NOTE: This is probably overkill, but it will work
  unsigned char buffer[buffer_size];
  //for almost all cases

  for(int VolumeIndex=0;VolumeIndex<VolumeSize;VolumeIndex++)
  {
    if (runlength==0)
    {
      CurrentObjIndex=SourceImage.ConstPixel(VolumeIndex);
      runlength=1;
    }
    else
    {
      if (CurrentObjIndex==SourceImage.ConstPixel(VolumeIndex))
      {
        runlength++;
        if (runlength==255)
        {
          buffer[bufferindex]=runlength;
          buffer[bufferindex+1]=CurrentObjIndex;
          bufferindex+=2;
          runlength=0;
        }
      }
      else
      {
        buffer[bufferindex]=runlength;
        buffer[bufferindex+1]=CurrentObjIndex;
        bufferindex+=2;
        CurrentObjIndex=SourceImage.ConstPixel(VolumeIndex);
        runlength=1;
      }
    }
    planeindex++;
    if (planeindex==PlaneSize)
    {
      // Just finished with a plane of data, so encode it
      planeindex=0;
      if (runlength!=0)
      {
        buffer[bufferindex]=runlength;
        buffer[bufferindex+1]=CurrentObjIndex;
        bufferindex+=2;
        runlength=0;
      }
    }
    if (bufferindex==buffer_size)
    {
      // buffer full
      if (fwrite(buffer,1,buffer_size,fptr)!=static_cast<unsigned int>(buffer_size))
      {
        printf("error: could not write buffer\n");
        exit(-1);
      }
      bufferindex=0;
    }

  }
  if (bufferindex!=0)
  {
    if (runlength!=0)
    {
      buffer[bufferindex]=runlength;
      buffer[bufferindex+1]=CurrentObjIndex;
      bufferindex+=2;
    }
    if (fwrite(buffer,1,bufferindex,fptr)!=static_cast<unsigned int>(bufferindex))
    {
      printf("error: could not write buffer\n");
      exit(-1);
    }
  }

  return true;
}


void CompactString( std::string & output, const std::string input )
{
  char *buffer = new char(input.length() + 1);
  int count = 0;

  for (unsigned int i = 0; i < input.length(); i++)
  {
    if (input[i] != ' ')
    {
      buffer[count] = input[i];
    }
  }

  buffer[input.length()] = '\0';

  output = std::string(buffer);

  delete[] buffer;
}
