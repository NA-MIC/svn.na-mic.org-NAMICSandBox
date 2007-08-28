#include "itkStochasticTractographyFilter.h"
#include "itkVectorImage.h"
#include "itkImageSeriesReader.h"  //this is needed for itk::ExposeMetaData()
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMetaDataDictionary.h"
#include "itkAddImageFilter.h"
#include "itkUnaryFunctorImageFilter.h"
#include <iostream>
#include <vector>
#include "itkImageRegionConstIterator.h"
#include "StochasticTractographyFilterCLP.h"
#include "itkTensorFractionalAnisotropyImageFilter.h"
#include "itkPathIterator.h"
#include <string>
#include <fstream>
#include "vtkPolyDataWriter.h"
#include "vtkPolyData.h"
#include "vtkCellArray.h"
#include "vtkPoints.h" 

int main(int argc, char* argv[]){
  PARSE_ARGS;
  //define the input/output types
  typedef itk::VectorImage< unsigned short int, 3 > DWIVectorImageType;
  typedef itk::Image< float, 3 > WMPImageType;
  typedef itk::Image< short int, 3 > ROIImageType;
  typedef itk::Image< unsigned int, 3 > CImageType;

  //define an iterator for the ROI image
  typedef itk::ImageRegionConstIterator< ROIImageType > ROIImageIteratorType;
  
  //define reader and writer
  typedef itk::ImageFileReader< DWIVectorImageType > DWIVectorImageReaderType;
  typedef itk::ImageFileReader< WMPImageType > WMPImageReaderType;
  typedef itk::ImageFileReader< ROIImageType > ROIImageReaderType;
  typedef itk::ImageFileWriter< CImageType > CImageWriterType;
  typedef itk::ImageFileWriter< WMPImageType > WMPImageWriterType;
  
  //define metadata dictionary types
  typedef itk::MetaDataDictionary DictionaryType;
  typedef DictionaryType::ConstIterator DictionaryIteratorType;
    
  //define a probabilistic tractography filter type and associated bValue,
  //gradient direction, and measurement frame types
  typedef itk::StochasticTractographyFilter< DWIVectorImageType, WMPImageType,
    ROIImageType >
    STFilterType;
  typedef STFilterType::bValueContainerType bValueContainerType;
  typedef STFilterType::GradientDirectionContainerType GDContainerType;
  typedef STFilterType::MeasurementFrameType MeasurementFrameType;
  
  //define AddImageFilterType to accumulate the connectivity maps of the pixels in the ROI
  typedef itk::AddImageFilter< CImageType, CImageType, CImageType> AddImageFilterType;
  
  //read in the DWI image
  DWIVectorImageReaderType::Pointer dwireaderPtr = DWIVectorImageReaderType::New();
  dwireaderPtr->SetFileName(dwifilename);
  dwireaderPtr->Update();
  
  //Obtain bValue, gradient directions and measurement frame from metadata dictionary
  DictionaryType& dictionary = dwireaderPtr->GetMetaDataDictionary();
  bValueContainerType::Pointer bValuesPtr = bValueContainerType::New();
  MeasurementFrameType measurement_frame;
  GDContainerType::Pointer gradientsPtr = GDContainerType::New();
  bValueContainerType::Element scaling_bValue = 0;
  GDContainerType::Element g_i;

  //bad choice of variable names: dictit->first refers to the key in the Map
  //dictit->second refers to the Value associated with that Key
  for(DictionaryIteratorType dictit = dictionary.Begin();
      dictit!=dictionary.End();
      ++dictit){
    if(dictit->first.find("DWMRI_gradient") != std::string::npos){
      std::string metaDataValue;
      itk::ExposeMetaData< std::string > (dictionary, dictit->first, metaDataValue);
      sscanf(metaDataValue.c_str(), "%lf %lf %lf\n", &g_i[0], &g_i[1], &g_i[2]);
      //normalize the gradient vector
      gradientsPtr->InsertElement( gradientsPtr->Size(), g_i.normalize() );
    }
    else if(dictit->first.find("DWMRI_b-value") != std::string::npos){
      std::string metaDataValue;
      itk::ExposeMetaData< std::string >(dictionary, dictit->first, metaDataValue);
      scaling_bValue = atof(metaDataValue.c_str());
    }
    else if(dictit->first.find("NRRD_measurement frame") != std::string::npos){
      std::vector< std::vector < double > > metaDataValue;
      itk::ExposeMetaData< std::vector< std::vector<double> > >
        (dictionary, dictit->first, metaDataValue);
      for(int i=0;i<3;i++){
        for(int j=0;j<3;j++)
          measurement_frame(i,j) = metaDataValue[j][i];
      }
    }
    else{
      std::cout << dictit->first << std::endl;
    }
  }
  
  //check to see if bValue, gradients, or measurement frame is missing
  if(scaling_bValue == 0){
    std::cerr << "scaling_bValue should never be 0, possibly not found in Nrrd header\n";
    return EXIT_FAILURE;
  }
  else if(gradientsPtr->Size() == 0){
    std::cerr <<"no gradients were found!";
    return EXIT_FAILURE;
  }
  else if(measurement_frame.size() == 0){
    std::cerr <<"no measurement frame was found!";
    return EXIT_FAILURE;
  }
 
  std::cout << scaling_bValue << std::endl;
  for(unsigned int i=0; i<gradientsPtr->Size(); i++)
    std::cout << gradientsPtr->GetElement(i) << std::endl;
  
  for(unsigned int i=0; i<measurement_frame.rows(); i++){
    for(unsigned int j=0; j<measurement_frame.columns(); j++){
      std::cout<<measurement_frame(i,j) << " ";
    }
    std::cout << std::endl;
  }
 
  //fill up bValue container with the scaling_bValue;
  for(unsigned int i=0; i<gradientsPtr->Size() ;i++){
    if(gradientsPtr->GetElement(i).squared_magnitude() == 0){
      bValuesPtr->InsertElement(i, 0);
    }
    else{
      //this matters in the calculations for the constrained model but not the tensor model
      //since a gradient direction of all zeros handles it
      bValuesPtr->InsertElement(i, scaling_bValue);
    }
  }
  
  //setup the ROI image reader
  ROIImageReaderType::Pointer roireaderPtr = ROIImageReaderType::New();
  roireaderPtr->SetFileName(roifilename);
  roireaderPtr->Update();
  
  //setup the white matter probability image reader
  WMPImageReaderType::Pointer wmpreader = WMPImageReaderType::New();
  wmpreader->SetFileName(wmpfilename);
  wmpreader->Update();
  
  //optionally set the origins of these images to be the same as the DWI
  if(recenteroriginswitch==true){
    roireaderPtr->GetOutput()->SetOrigin( dwireaderPtr->GetOutput()->GetOrigin() );
    wmpreader->GetOutput()->SetOrigin( dwireaderPtr->GetOutput()->GetOrigin() );
  }
  std::cout<<"DWI image origin:"<< dwireaderPtr->GetOutput()->GetOrigin() <<std::endl;
  std::cout<<"ROI image origin:"<< roireaderPtr->GetOutput()->GetOrigin() <<std::endl;
  std::cout<<"wmp image origin:"<< wmpreader->GetOutput()->GetOrigin() <<std::endl;

  std::cout<<"Create STFilter\n";
  //Setup the Stochastic Tractography Filter
  STFilterType::Pointer stfilterPtr = STFilterType::New();
  stfilterPtr->SetDWIImageInput( dwireaderPtr->GetOutput() );
  stfilterPtr->SetWhiteMatterProbabilityImageInput( wmpreader->GetOutput() );
  stfilterPtr->SetROIImageInput( roireaderPtr->GetOutput() );
  stfilterPtr->SetbValues(bValuesPtr);
  stfilterPtr->SetGradients( gradientsPtr );
  stfilterPtr->SetMeasurementFrame( measurement_frame );
  stfilterPtr->SetMaxTractLength( maxtractlength );
  stfilterPtr->SetTotalTracts( totaltracts );
  stfilterPtr->SetMaxLikelihoodCacheSize( maxlikelihoodcachesize );
  stfilterPtr->SetStepSize( stepsize );
  stfilterPtr->SetROILabel( labelnumber );
  stfilterPtr->SetGamma( gamma );
  if(totalthreads!=0) stfilterPtr->SetNumberOfThreads( totalthreads );
  
  stfilterPtr->Update();
  STFilterType::TractContainerType::Pointer tractcontainer = 
    stfilterPtr->GetOutputDiscreteTractContainer();
  
  //allocate the VTK Polydata to output the tracts
  vtkPolyData* vtktracts = vtkPolyData::New();
  vtkPoints* points = vtkPoints::New();
  vtkCellArray* vtktractarray = vtkCellArray::New();
  
  for(int i=0; i<tractcontainer->Size(); i++ ){
    STFilterType::TractContainerType::Element tract =
      tractcontainer->GetElement(i);

    STFilterType::TractContainerType::Element::ObjectType::VertexListType::ConstPointer vertexlist = 
      tract->GetVertexList();
        
    //create a new cell
    vtktractarray->InsertNextCell(vertexlist->Size());

    for(int j=0; j<vertexlist->Size(); j++){
      STFilterType::TractContainerType::Element::ObjectType::VertexListType::Element vertex =
        vertexlist->GetElement(j);
          
      points->InsertNextPoint(vertex[0], vertex[1], vertex[2]);
      vtktractarray->InsertCellPoint(points->GetNumberOfPoints()-1);
    }
  }        
  
  //finish up the vtk polydata
  vtktracts->SetPoints( points );
  vtktracts->SetLines( vtktractarray );
  //output the vtk tract container
  vtkPolyDataWriter* vtktractswriter = vtkPolyDataWriter::New();
  vtktractswriter->SetInput( vtktracts );
  std::string tractsfilename = outputprefix + "_TRACTS.vtk";
  vtktractswriter->SetFileName( tractsfilename.c_str() );
  vtktractswriter->Write();
  if(outputimageswitch){
    //Setup the AddImageFilter
    AddImageFilterType::Pointer addimagefilterPtr = AddImageFilterType::New();
    
    //Create a temporary Connectivity Image
    CImageType::Pointer tempcimagePtr = CImageType::New();
    tempcimagePtr->CopyInformation( dwireaderPtr->GetOutput() );
    tempcimagePtr->SetBufferedRegion( dwireaderPtr->GetOutput()->GetBufferedRegion() );
    tempcimagePtr->SetRequestedRegion( dwireaderPtr->GetOutput()->GetRequestedRegion() );
    tempcimagePtr->Allocate();
    tempcimagePtr->FillBuffer(0);
    
    //Create a zeroed accumulated Connectivity Image
    CImageType::Pointer accumulatedcimagePtr = CImageType::New();
    accumulatedcimagePtr->CopyInformation( dwireaderPtr->GetOutput() );
    accumulatedcimagePtr->SetBufferedRegion( dwireaderPtr->GetOutput()->GetBufferedRegion() );
    accumulatedcimagePtr->SetRequestedRegion( dwireaderPtr->GetOutput()->GetRequestedRegion() );
    accumulatedcimagePtr->Allocate();
    accumulatedcimagePtr->FillBuffer(0);
      
    //graft this onto the output of the addimagefilter
    addimagefilterPtr->GraftOutput( accumulatedcimagePtr );
    addimagefilterPtr->SetInput1( tempcimagePtr );
    addimagefilterPtr->SetInput2( addimagefilterPtr->GetOutput() );
    
    //write tracts to connectivity image
    for(int i=0; i<tractcontainer->Size(); i++ ){
      tempcimagePtr->FillBuffer(0);
      STFilterType::TractContainerType::Element tract =
        tractcontainer->GetElement(i);
         
      STFilterType::TractContainerType::Element::ObjectType::VertexListType::ConstPointer vertexlist = 
        tract->GetVertexList();
      
      for(int j=0; j<vertexlist->Size(); j++){
        STFilterType::TractContainerType::Element::ObjectType::VertexListType::Element vertex =
          vertexlist->GetElement(j);
        CImageType::IndexType index;
        index[0]=static_cast<long int>(vertex[0]);
        index[1]=static_cast<long int>(vertex[1]);
        index[2]=static_cast<long int>(vertex[2]);
        
        CImageType::PixelType& tempcimagepix = tempcimagePtr->GetPixel( index );
        if(tempcimagepix == 0){
          tempcimagepix++;
        }
      }
      addimagefilterPtr->Update();
    }
    
    //Write out the Connectivity Map
    std::string cfilename = outputprefix + "_CMAP.nhdr";

    CImageWriterType::Pointer writerPtr = CImageWriterType::New();
    writerPtr->SetInput( accumulatedcimagePtr );
    writerPtr->SetFileName( cfilename.c_str() );
    writerPtr->Update();
  }
  return EXIT_SUCCESS;
}
