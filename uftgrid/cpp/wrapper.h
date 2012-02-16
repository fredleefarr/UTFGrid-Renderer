 #pragma once
  
#include <string>

 using namespace System;
 using namespace System::Collections::Generic;
 using namespace System::Drawing;
 using namespace model::types;
  
 namespace wrapper {
  
     public ref class GridRenderer
     {
     public:
         UtfGrid^ GetUtfGrid(TileUtfGridCollection ^utfGridCollection);
     };
 }