int main(int argc, char * argv[]){

  //first arg == ID
  //second arg == contact ID
  //third arg == contact IP
  if (argc == 2){ //creating new network
    Node n(argv[1]); //assume convert to number
    n.startListener();
  }else if (argc == 4){
    Node n(argv[1], argv[2], argv[2]);
    if (n.joined()){
      n.startListener();
    }
    else{
      //ERROR OUT
    }
  }
  
  return 0;
}
