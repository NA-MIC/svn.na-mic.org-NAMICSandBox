/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageTest.cxx,v $
  Language:  C++
  Date:      $Date: 2005/04/11 13:52:36 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkMaximumRatioDecisionRule2.h"
#include "itkObjectFactory.h"

int itkMaximumRatioDecisionRuleTest(int, char* [] )
{
  typedef itk::Statistics::MaximumRatioDecisionRule2      MaximumRatioDecisionRuleType;

  typedef MaximumRatioDecisionRuleType::MembershipVectorType MembershipVectorType;

  MaximumRatioDecisionRuleType::Pointer decisionRule = MaximumRatioDecisionRuleType::New();

  std::cout << decisionRule->GetNameOfClass() << std::endl;
  std::cout << decisionRule->MaximumRatioDecisionRuleType::Superclass::GetNameOfClass() << std::endl;

  decisionRule->Print(std::cout);

  MembershipVectorType membershipScoreVector;
  
  double membershipScore1;
  membershipScore1 = 0.1;
  membershipScoreVector.push_back( membershipScore1 );  

  double membershipScore2;
  membershipScore2 = 0.5;
  membershipScoreVector.push_back( membershipScore2 );  

  double membershipScore3;
  membershipScore3 = 1.9;
  membershipScoreVector.push_back( membershipScore3 );  

  // the maximum score is the third component. The decision rule should
  // return index ( 2) 
  if( decisionRule->Evaluate( membershipScoreVector ) != 2 )
    {
      std::cerr << "Decision rule computation is incorrect!" << std::endl;
      return EXIT_FAILURE;
    } 

  return EXIT_SUCCESS;
}
