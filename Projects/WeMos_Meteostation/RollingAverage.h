class RollingAverage{
  private:
    const int size = 50;
    const int empty_value = -100;
    float values[50];
  
  public:
  RollingAverage(){
    for(int i = 0; i<size; i++){
      values[i] = empty_value;
    }
  }

  void add(float value){
    for (int i=size-1;i>0;i--){
      values[i] = values[i-1];
    }

    values[0] = value;
  }

  float getAverage(){
    float sum = 0;
    int count = 0;

    for(int i=0;i<size;i++){

      if (values[i] == empty_value){

        if (i == 0){
          return empty_value;
        }
        
        break;
      }

      sum += values[i];      
      count = i+1;
    }
    
    return sum/count;
  }

  String getArrayString(){
    String result = "[";
    for(int i = 0; i < size; i++){
        result += (String(values[i]) + " ").c_str();
      }
    result += "]";

    return result;
  }
};
