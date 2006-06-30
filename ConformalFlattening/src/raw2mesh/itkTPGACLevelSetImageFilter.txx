#ifndef __itkTPGACLevelSetImageFilter_txx_
#define __itkTPGACLevelSetImageFilter_txx_

#include "itkTPGACLevelSetImageFilter.h"

namespace itk {


template <class TInputImage, class TFeatureImage, class TOutputType>
TPGACLevelSetImageFilter< TInputImage, TFeatureImage, TOutputType>
::TPGACLevelSetImageFilter() :
//  GeodesicActiveContourLevelSetImageFilter<
//  TInputImage, TFeatureImage, TOutputType>::
GeodesicActiveContourLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>() 

// call parent ctor
{

}
 
template <class TInputImage, class TFeatureImage, class TOutputType>
void TPGACLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

// 6-neighbour table (including centre voxel, i.e. voxel 13)
static int nbh6Table[27][6] = {
  {1, 3, 9, -1, -1, -1},    // 0
  {0, 2, 4, 10, -1, -1},    // 1
  {1, 5, 11, -1, -1, -1},   // 2
  {0, 4, 6, 12, -1, -1},    // 3
  {1, 3, 5, 7, 13, -1},     // 4
  {2, 4, 8, 14, -1, -1},    // 5
  {3, 7, 15, -1, -1, -1},   // 6
  {4, 6, 8, 16, -1, -1},    // 7
  {5, 7, 17, -1, -1, -1},   // 8
  
  {0, 10, 12, 18, -1, -1},  // 9
  {1, 9, 11, 13, 19, -1},   // 10
  {2, 10, 14, 20, -1, -1},  // 11
  {3, 9, 13, 15, 21, -1},   // 12
  {4, 10, 12, 14, 16, 22},  // 13
  {5, 11, 13, 17, 23, -1},  // 14
  {6, 12, 16, 24, -1, -1},  // 15
  {7, 13, 15, 17, 25, -1},  // 16
  {8, 14, 16, 26, -1, -1},  // 17

  {9, 19, 21, -1, -1, -1},  // 18
  {10, 18, 20, 22, -1, -1}, // 19
  {11, 19, 23, -1, -1, -1}, // 20
  {12, 18, 22, 24, -1, -1}, // 21
  {13, 19, 21, 23, 25, -1}, // 22
  {14, 20, 22, 26, -1, -1}, // 23
  {15, 21, 25, -1, -1, -1}, // 24
  {16, 22, 24, 26, -1, -1}, // 25
  {17, 23, 25, -1, -1, -1}  // 26
};

// generated by gen26neighbourTable.py
// includes the centre voxel
static int nbh26Table[27][26] = {
  {1, 3, 4, 9, 10, 12, 13, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 0
  {0, 2, 3, 4, 5, 9, 10, 11, 12, 13, 14, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 1
  {1, 4, 5, 10, 11, 13, 14, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 2
  {0, 1, 4, 6, 7, 9, 10, 12, 13, 15, 16, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 3
  {0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
   14, 15, 16, 17, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 4
  {1, 2, 4, 7, 8, 10, 11, 13, 14, 16, 17, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 5
  {3, 4, 7, 12, 13, 15, 16, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 6
  {3, 4, 5, 6, 8, 12, 13, 14, 15, 16, 17, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 7
  {4, 5, 7, 13, 14, 16, 17, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 8
  
  {0, 1, 3, 4, 10, 12, 13, 18, 19, 21, 22, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 9
  {0, 1, 2, 3, 4, 5, 9, 11, 12, 13, 14, 18, 19, 
   20, 21, 22, 23, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 10
  {1, 2, 4, 5, 10, 13, 14, 19, 20, 22, 23, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 11
  {0, 1, 3, 4, 6, 7, 9, 10, 13, 15, 16, 18, 19, 
   21, 22, 24, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 12
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
   14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26},  // 13
  {1, 2, 4, 5, 7, 8, 10, 11, 13, 16, 17, 19, 20, 
   22, 23, 25, 26, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 14
  {3, 4, 6, 7, 12, 13, 16, 21, 22, 24, 25, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 15
  {3, 4, 5, 6, 7, 8, 12, 13, 14, 15, 17, 21, 22, 
   23, 24, 25, 26, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 16
  {4, 5, 7, 8, 13, 14, 16, 22, 23, 25, 26, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 17
  
  {9, 10, 12, 13, 19, 21, 22, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 18
  {9, 10, 11, 12, 13, 14, 18, 20, 21, 22, 23, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 19
  {10, 11, 13, 14, 19, 22, 23, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 20
  {9, 10, 12, 13, 15, 16, 18, 19, 22, 24, 25, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 21
  {9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 
   23, 24, 25, 26, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 22
  {10, 11, 13, 14, 16, 17, 19, 20, 22, 25, 26, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 23
  {12, 13, 15, 16, 21, 22, 25, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 24
  {12, 13, 14, 15, 16, 17, 21, 22, 23, 24, 26, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // 25
  {13, 14, 16, 17, 22, 23, 25, -1, -1, -1, -1, -1, -1, 
   -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}  // 26
};






/**
 * Checks if the centre'th element of srcNbh is on.  If it is,
 * activate that element in dstNbh and also i0 to i3 if they are
 * active in srcNbh.  This is used during the recursive 6 connectivity
 * determination.
 */
static inline void fillLocal6Neighbours(int *srcNbh, int *dstNbh, int centre,
                                        int i0, int i1, int i2, int i3)
{
  if (srcNbh[centre])
    {
    dstNbh[centre] = 1;

    if (srcNbh[i0])
      dstNbh[i0] = 1;
    
    if (srcNbh[i1])
      dstNbh[i1] = 1;

    if (srcNbh[i2])
      dstNbh[i2] = 1;

    if (srcNbh[i3])
      dstNbh[i3] = 1;    
    }
    
}

/**
 * The idx'th voxel in nbh is ALREADY labeled.  This checks for
 * existing 6-neighbours and gives them label curlabel.
 */
static void label6Neighbours(int *nbh, int *nbhlabels, int *nbhv,
                             int curlabel, int idx)
{
  // needs good initial value
  int nbhIdx = 0;

  // 6 neighbours max (also in the lookup table)
  for (int i = 0; i < 6 && nbhIdx >= 0; i++)
    {
    nbhIdx = nbh6Table[idx][i];
    // valid nbh index and the voxel exists and it hasn't been labeled
    // yet
    if (nbhIdx >= 0 && nbh[nbhIdx] && nbhlabels[nbhIdx] == 0)
      {
      // then label it
      nbhlabels[nbhIdx] = curlabel;
      // and record that it has been labeled, but needs to recursed
      // we only do this if V doesn't have a value yet
      if (nbhv[nbhIdx] == 0)
        nbhv[nbhIdx] = 1;
      }
    }
}

/**
 * The idx'th voxel in nbh is ALREADY labeled.  This checks for
 * existing 26-neighbours and gives them label curlabel.
 */
static void label26Neighbours(int *nbh, int *nbhlabels, int *nbhv,
                              int curlabel, int idx)
{
  // needs good initial value
  int nbhIdx = 0;

  // 26 neighbours max (also in the lookup table)
  for (int i = 0; i < 26 && nbhIdx >= 0; i++)
    {
    nbhIdx = nbh26Table[idx][i];
    // valid nbh index and the voxel exists and it hasn't been labeled
    // yet
    if (nbhIdx >= 0 && nbh[nbhIdx] && nbhlabels[nbhIdx] == 0)
      {
      // then label it
      nbhlabels[nbhIdx] = curlabel;
      // and record that it has been labeled, but needs to recursed
      // we only do this if V doesn't have a value yet
      if (nbhv[nbhIdx] == 0)
        nbhv[nbhIdx] = 1;
      }
    }
}


static inline int connectedComponents(
  int *nbh, int *nbhLabels,
  void (*labelNeighboursFunc)(int *, int *, int *, int, int)
  )
{
  // create and init V struct
  int nbhV[27];
  memset(nbhV, 0, 27 * sizeof(int));                          

  int curlabel = 1, assignedlabel = 0;
  for (int i = 0; i < 27; i++)
    {
    // is there a voxel at this position, and has it not been labeled yet?
    if (nbh[i] && nbhLabels[i] == 0)
      {
      // ON voxel not labeled yet
      nbhLabels[i] = curlabel;
      // this is to keep track of how many labels we've actually USED
      assignedlabel = curlabel;
      // mark it as being labeled
      nbhV[i] = 1; 

      // now recurse through n26v finding ALL voxels of curlabel
      // we continue doing this until there are no 1s, i.e. only
      // 2s (neighbours examined) and 0s (no connected labels)
      int onesFound;
      do
        {
        onesFound = 0;
        for (int j = 0; j < 27; j++)
          {
          if (nbhV[j] == 1)
            {
            onesFound = 1;
            // this will label 6-neighbours and also flag the fact
            // that they're labeled by setting a '1' in n26v
            // neighbours that are already 2 will be left alone
            
            labelNeighboursFunc(nbh, nbhLabels, nbhV, curlabel, j);
            // now all neighbours of voxel j have also been labeled
            nbhV[j] = 2;
            }
          } // for (int j = 0 ...
        }
      while (onesFound);


      // if we find the next unlabeled thing, it has to be a new
      // component by definition
      curlabel++;
      
      } // if (n26nbh[i] && n26labels[i] == 0) ...
    } // for (int i = 0; i < 27 ...

  return assignedlabel;
  
}

// you could also use epsilon from the levelset function
#define TPGAC_EPSILON 1e-5;

template <class TInputImage, class TFeatureImage, class TOutputType>
typename TPGACLevelSetImageFilter<
  TInputImage, TFeatureImage, TOutputType>::ValueType
  TPGACLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
  ::CalculateUpdateValue(const IndexType &idx,   const TimeStepType &dt,
                         const ValueType &value, const ValueType &change)
{

  // * calculate new value
  // * if new value has the same sign as current value, make the
  //   change
  // * ELSE:
  // * extract 3x3x3 neighbourhood of the current voxel
  // * calculate N^2_6(x,X) and N^1_26(x,X')
  // * count connected components (bail if more than 1)
  // * if both 1, then x is simple point, allow change
  // * if not (or bailed) x is not simple point
  // * newValue = epsilon * sign(value) (epsilon small and positive)

  ValueType temp_value = value + dt * change;

  // sign is the same, we can return what we have
  if (temp_value * value >= 0)
    {
    return temp_value;
    }
  
  // create a 3x3x3 nbh iterator over the output image
  Size<3> radius = {1,1,1};
  NeighborhoodIterator<OutputImageType>
    nbhIterator(radius, this->GetOutput(),
                this->GetOutput()->GetRequestedRegion());

  // move the 3x3x3 nbh iterator over the current voxel
  nbhIterator.SetLocation(idx);

  // offset of centre pixel
#define c 13

  // transfer nbh to our interior/exterior nbh
  int ieNbh[27];
  for (int i = 0; i < 27; i++)
    {
    if (nbhIterator.GetPixel(i) >= 0)
      {
      // interior / inside / foreground
      ieNbh[i] = 1;
      }
    else
      {
      // exterior / outside / background
      ieNbh[i] = 0;
      }
    }  

  // N^2_6 == n26
  // N^1_26 == n126
  
  // now calculate N^2_6(interior) - we do this as straight-forward as
  // possible for speed reasons
  // first allocate and clear the nbh array
  int n26nbh[27];
  memset(n26nbh, 0, 27 * sizeof(int));


//   if (ieNbh[4])
//     {
//     n26nbh[4] = 1;

//     if (ieNbh[1]) n26nbh[1] = 1;
//     if (ieNbh[3]) n26nbh[3] = 1;
//     if (ieNbh[5]) n26nbh[5] = 1;
//     if (ieNbh[7]) n26nbh[7] = 1;
//     }

  
  // then check the 6-neighbours of 4, i.e. 1, 3, 5, 7, but NOT the
  // center voxel itself... that's explicitly excluded
  fillLocal6Neighbours(ieNbh, n26nbh, 4, 1, 3, 5, 7);
  fillLocal6Neighbours(ieNbh, n26nbh, 10, 1, 9, 11, 19);
  fillLocal6Neighbours(ieNbh, n26nbh, 12, 3, 9, 15, 21);
  fillLocal6Neighbours(ieNbh, n26nbh, 14, 5, 11, 17, 23);
  fillLocal6Neighbours(ieNbh, n26nbh, 16, 7, 15, 17, 25);
  fillLocal6Neighbours(ieNbh, n26nbh, 22, 19, 21, 23, 25);

  // we should have a complete n^2_6(x,X) now...
  // now determine number of connected components using
  // fast method described in borgefors1997

  int n26labels[27];
  memset(n26labels, 0, 27 * sizeof(int));

  int ncc6 = connectedComponents(
    n26nbh, n26labels,
    label6Neighbours);
  
  if (ncc6 != 1)
    {
    // already T6(x,X) != 1, so we bail with epsilon * sign of old
    // value... this saves us from the 26-neighbourhood background check
    if (value < 0)
      {
      return -1 * TPGAC_EPSILON;
      }
    else
      {
      return TPGAC_EPSILON;
      }
    }

  int n126nbh[27];
  memset(n126nbh, 0, 27 * sizeof(int));

  // we just invert ieNbh, because we're going to check the background
  for (int i = 0; i < 27; i++)
    {
    n126nbh[i] = ! ieNbh[i];
    }

  // the centre voxel is NEVER used
  n126nbh[13] = 0;

  int n126labels[27];
  memset(n126labels, 0, 27 * sizeof(int));

  int ncc26 = connectedComponents(
    n126nbh, n126labels,
    label26Neighbours);

  if (ncc26 != 1)
    {
    // T26(x,X') != 1, so we bail with epsilon * sign of old
    // value... 
    if (value < 0)
      {
      return -1 * TPGAC_EPSILON;
      }
    else
      {
      return TPGAC_EPSILON;
      }
    }
  
  // this means the voxel that is to be added is simple... we can just
  // return the new value
  return temp_value;
  
  
}


}// end namespace itk

#endif
