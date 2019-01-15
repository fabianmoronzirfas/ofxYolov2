#include "ofxYolov2.h"


string Replace( string String1, string String2, string String3 )
{
    string::size_type  Pos( String1.find( String2 ) );

    while( Pos != string::npos )
    {
        String1.replace( Pos, String2.length(), String3 );
        Pos = String1.find( String2, Pos + String3.length() );
    }

    return String1;
}

ofxYolov2::ofxYolov2()
{

}


ofxYolov2::~ofxYolov2()
{

}


void ofxYolov2::setup(string _path_to_cfg, string _path_to_weights, string _path_to_names)
{
//    putenv("OPENCV_OPENCL_RUNTIME=");
//    putenv("OPENCV_OPENCL_DEVICE=:DGPU:0");

    if (!cv::ocl::haveOpenCL())
    {
        cout << "OpenCL is not available..." << endl;
        //return;
    }
    //cv::ocl::getPlatfomsInfo();

    cv::ocl::Context context;
    if (!context.create(cv::ocl::Device::TYPE_GPU))
    {
        cout << "Failed creating the context..." << endl;
        //return;
    }

    cout << context.ndevices() << " GPU devices are detected." << endl; //This bit provides an overview of the OpenCL devices you have in your computer
    for (int i = 0; i < context.ndevices(); i++)
    {
        cv::ocl::Device device = context.device(i);

        cout << "name:              " << device.name() << endl;
        cout << "available:         " << device.available() << endl;
        cout << "imageSupport:      " << device.imageSupport() << endl;
        cout << "OpenCL_C_Version:  " << device.OpenCL_C_Version() << endl;
        cout << endl;
    }
    //cv::ocl::setUseOpenCL( true );

    String modelConfiguration = _path_to_cfg;
    String modelBinary = _path_to_weights;

    //! [Initialize network]
    net = readNetFromDarknet(modelConfiguration, modelBinary);
    std::vector<String> lname = net.getLayerNames();
    for (int i = 0; i < lname.size();i++) {
        std::cout << i+1 << " " << lname[i] << std::endl;
    }
    net.setPreferableBackend(DNN_BACKEND_DEFAULT);
    net.setPreferableTarget(DNN_TARGET_CPU);


    if (net.empty())
    {
        cout << "Can't load network by using the following files: " << endl;
        cout << "cfg-file:     " << modelConfiguration << endl;
        cout << "weights-file: " << modelBinary << endl;
        cout << "Models can be downloaded here:" << endl;
        cout << "https://pjreddie.com/darknet/yolo/" << endl;
    }

    // objectClassName
    cout << _path_to_names << endl;
    ifstream classNamesFile(_path_to_names);
    if (classNamesFile.is_open())
    {
        string className = "";
        while (std::getline(classNamesFile, className)){
            classNamesVec.push_back(className);
        }

        for( auto itr : classNamesVec )
        {
            string cName = itr;
            //cout << "classNames :" << cName << endl;
        }
    }

    // set default Detection Color
    for( int i = 0; i < classNamesVec.size(); i++ ){
        detection_color.push_back(ofColor(100*((i+1)%2)+100,
                                          50*((i+2)%3)+100,
                                          25*((i+3)%4)+100,
                                          255));
    }

    // for gocen detection color
    /*
    {
        ofColor c[] =
        {
            ofColor(255, 255, 255),
            ofColor(111, 12, 188),
            ofColor(25, 71, 175),
            ofColor(188, 12, 180),
            ofColor(200, 255, 255),
            ofColor(255, 230, 51),
            ofColor(34, 181, 115),
            ofColor(98, 160, 0),
            ofColor(64, 109, 5),
            ofColor(255, 137, 57),
            ofColor(255, 85, 85),
            ofColor(255, 102, 186),
            ofColor(140, 98, 57),
            ofColor(238, 118, 255),
            ofColor(117, 158, 255)
        };

        for( int i = 0; i < classNamesVec.size(); i++){
            detection_color.push_back(c[i]);
        }
    }
     */

    font_info.load(ofToDataPath("DIN Alternate Bold.ttf"),12);
    confidenceThreshold = 0.6;
    class_id_selected = 0; // default
}



void ofxYolov2::drawAnnotation(float _x, float _y, float _w, float _h)
{
    ofSetColor(255);
    if( image_annotation.isAllocated() ){
        image_annotation.draw(0,0, _w, _h);
    }

    for( int i = 0; i < train.size(); i++ ){
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(detection_color.at(train[i].id),150);
        ofRectangle r_scaled = train.at(i).getScaledBB(_w, _h);

        ofDrawRectangle(r_scaled);

        ofFill();
        ofDrawRectangle(r_scaled.x, r_scaled.y-18,r_scaled.width,18);
        ofSetColor(ofColor::white);
        font_info.drawString("["+ofToString(train.at(i).id)+"]: "+train.at(i).name,                          r_scaled.x,r_scaled.y);

    }


    for( int i = 0; i < train.size(); i++ ){
        ofSetColor(detection_color[train[i].id],150);
        if( train[i].getScaledBB(image_annotation.getWidth(), image_annotation.getHeight()). inside(ofGetMouseX(), ofGetMouseY()) ){
            ofFill();
            ofDrawRectangle(train[i].getScaledBB(image_annotation.getWidth(), image_annotation.getHeight()));
        }
    }

}

cv::Mat ofxYolov2::toCV(ofPixels &pix)
{

    return cv::Mat(pix.getHeight(), pix.getWidth(), CV_MAKETYPE(CV_8U, pix.getNumChannels()), pix.getData(), 0);
}


void ofxYolov2::draw(float _x, float _y, float _w, float _h) {

    for( int i = 0; i < objects.size(); i++ ){
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(detection_color.at(objects[i].class_id));
        ofRectangle r_scaled = objects.at(i).getScaledBB(_w, _h);
        ofDrawRectangle(r_scaled);

        ofFill();
        ofDrawRectangle(r_scaled.x, r_scaled.y-18,r_scaled.width,18);
        ofSetColor(ofColor::white);
        font_info.drawString("["+ofToString(objects.at(i).class_id)+"]: "+objects.at(i).name + ": " + ofToString(objects.at(i).confidence),
                                    r_scaled.x,r_scaled.y);
    }

    for(auto pp: person_persistance_list){
        // pp.draw();

        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(ofColor::tomato);
        ofRectangle rect_scaled = pp.getScaledBB(_w, _h);
        // cout << pp.id << endl;
        cout << "X: " << pp.rect.getX() << " Y: " << pp.rect.getY() << " w: " << pp.rect.getWidth() << " h: " << pp.rect.getHeight() << endl;
        ofDrawRectangle(rect_scaled);
    }
}

void ofxYolov2::update(ofPixels &op) {
    // Scenario 1 Personlist is empty

    objects.clear();
    persons.clear();

    cv::Mat frame = toCV(op);

    if (frame.channels() == 4)
        cvtColor(frame, frame, COLOR_BGRA2BGR);

    //! [Resizing without keeping aspect ratio]
    Mat resized;
    resize(frame, resized, cvSize(network_width, network_height));

    //! [Prepare blob]
    Mat inputBlob = blobFromImage(resized, 1 / 255.F); //Convert Mat to batch of images

    //! [Set input blob]
    net.setInput(inputBlob, "data");                   //set the network input

    std::vector<Mat> outs;
    //! [Make forward pass]
    Mat detectionMat;
    if(yolo_version == 2){
        detectionMat = net.forward("detection_out");   //compute output for yolov2
    }else if(yolo_version == 3){
        detectionMat = net.forward("yolo_23");   //compute output for yolov3
    }

    for (int i = 0; i < detectionMat.rows; i++) {

        const int probability_index = 5; // do not change.
        const int probability_size = detectionMat.cols - probability_index;
        float *prob_array_ptr = &detectionMat.at<float>(i, probability_index);


        size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;
        float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);
        if(confidence > 0){

        // std::cout << "Confidence: " << confidence << endl;
        // std::cout << "Class: " << String(classNamesVec[objectClass]) << endl;
        }
        if (confidence > confidenceThreshold)
        {
            float x = detectionMat.at<float>(i, 0);
            float y = detectionMat.at<float>(i, 1);
            float width = detectionMat.at<float>(i, 2);
            float height = detectionMat.at<float>(i, 3);
            int xLeftBottom = static_cast<int>((x - width / 2) * frame.cols);
            int yLeftBottom = static_cast<int>((y - height / 2) * frame.rows);
            int xRightTop = static_cast<int>((x + width / 2) * frame.cols);
            int yRightTop = static_cast<int>((y + height / 2) * frame.rows);


            if (objectClass < classNamesVec.size())
            {
                String label = String(classNamesVec[objectClass]);
                ofRectangle r(x-width/2.0,y-height/2.0,width,height);
                objects.push_back(Object(objectClass, label, confidence, r.x,r.y, r.width, r.height));

            }
            else
            {
                cout << "Class: " << objectClass << endl;
                cout << "Confidence: " << confidence << endl;
                cout << " " << xLeftBottom
                << " " << yLeftBottom
                << " " << xRightTop
                << " " << yRightTop << endl;
            }
        }
    } // end if detection
    // filter objects for persons
    for(const auto& obj: objects){
        if(obj.class_id == 0) {
            persons.push_back(obj);
        }
    }
    // debugging output
    // cout << "Persons list" << endl;
    // for(const auto& person: persons){
    //     cout << person.name << " " << person.confidence << " ";
    // }
    // cout << endl;
    // We dont have any perons in our list
    // add them
    if(person_persistance_list.empty()){
        for(const auto& person: persons){
            person_persistance_list.push_back(
                Person(
                    person.r.getX(),
                    person.r.getY(),
                    person.r.getWidth(),
                    person.r.getHeight(),
                    getPersonCount()));
                    // cout << person_count << "\tperson_count" << endl;
                    // cout << person.r.getX() << "\tperson.r.getX()" << endl;
                    // cout << person.r.getY() << "\tperson.r.getY()" << endl;
                    // cout << person.r.getWidth() << "\tperson.r.getWidth()" << endl;
                    // cout << person.r.getHeight() << "\tperson.r.getHeight()" << endl;
        }
    }else if(person_persistance_list.size()<=persons.size()){
        vector<bool> used(persons.size());
        for(auto& pp: person_persistance_list){
            float record = 50000;
            int index = -1;
            for (int i = 0; i < persons.size(); i++){
                float d = ofDist(
                    persons[i].r.getX(),
                    persons[i].r.getY(),
                    pp.rect.getX(),
                    pp.rect.getY());

                    if(d < record && !used[i]){
                        record = d;
                        index = i;
                    }
            }
            used[index] = true;
            pp.update(persons[index].r);

            // cout << " update existing" << endl;
            // cout << pp.id << "\t\tpp.id" << endl;
            // cout << pp.rect.getX() << "\tpp.rect.getX()" << endl;
            // cout << pp.rect.getY() << "\tpp.rect.getY()" << endl;
            // cout << pp.rect.getWidth() << "\tpp.rect.getWidth()" << endl;
            // cout << pp.rect.getHeight() << "\tpp.rect.getHeight()" << endl;
            // cout << endl;

            // cout << persons[index].r.getX() << "\tpersons[index].r.getX()" << endl;
            // cout << persons[index].r.getY() << "\tpersons[index].r.getY()" << endl;
            // cout << persons[index].r.getWidth() << "\tpersons[index].r.getWidth()" << endl;
            // cout << persons[index].r.getHeight() << "\tpersons[index].r.getHeight()" << endl;
            // cout << endl;

        }
        // add unused persons
        for(int i = 0; i < persons.size();i++){
            if(used[i] != true){
                person_persistance_list.push_back(Person(
                    persons[i].r.getX(),
                    persons[i].r.getY(),
                    persons[i].r.getWidth(),
                    persons[i].r.getHeight(),
                    getPersonCount()));
                    // cout << "add new ones pp with id "<< person_count << endl;
                    // cout << person_count <<" person_count"<< endl;
                    // cout << persons[i].r.getX() << "\tpersons[i].r.getX()" << endl;
                    // cout << persons[i].r.getY() << "\tpersons[i].r.getY()" << endl;
                    // cout << persons[i].r.getWidth() << "\tpersons[i].r.getWidth()" << endl;
                    // cout << persons[i].r.getHeight() << "\tpersons[i].r.getHeight()" << endl;

            }
        }
    // SCENARIO 3: We have more Face objects than face Rectangles found
    // removing old ones that are not in use anymore
    }else{
        for (auto& pp : person_persistance_list) {
            pp.available = true;
        }
        for(int i = 0; i < persons.size(); i++){
            float record = 50000;
            int index = -1;
            for(int j = 0; j < person_persistance_list.size(); j++){
                Person pp = person_persistance_list[j];
                float d = ofDist(
                    persons[i].r.getX(),
                    persons[i].r.getY(),
                    pp.rect.getX(),
                    pp.rect.getY()
                );
                if(d < record && pp.available == true){
                    record = d;
                    index = j;
                }
            }
            Person pp = person_persistance_list[index];
            pp.available = false;
            pp.update(persons[i].r);
        }
        for(auto& pp: person_persistance_list){
            if(pp.available){
                pp.countDown();
                cout << "timer: " << pp.timer << endl;
                if(pp.dead()){
                    pp.del = true;
                }
            }
        }
    }
    for(int i = person_persistance_list.size()-1; i>=0;i--){
        Person pp = person_persistance_list[i];
        cout << " should i delete? " << pp.del << endl;
        if(pp.del == true){
            cout << "removing person at index " << i << endl;
            person_persistance_list.erase(person_persistance_list.begin() + i);
        }
    }

}
int ofxYolov2::getPersonCount(){
    int val = person_count;
    person_count++;
    cout << " old " << val << " new " << person_count<< endl;
    return val;
}

void ofxYolov2::setConfidenceThreshold(float _threshold)
{
    if( 0.0 <= _threshold && _threshold <= 1.0){
        confidenceThreshold = _threshold;
    }
}

void ofxYolov2::loadBoundingBoxFile(string _path_to_file)
{
    train.clear();
    vector<string>str_bb;

    if( !ofFile::doesFileExist(_path_to_file) ){
        cout << "no such a file" << endl;
        ofSystem("touch "+_path_to_file);
    }
    ofBuffer ofbuf = ofBufferFromFile(_path_to_file);

    ofbuf.getLines();
    for( auto line: ofbuf.getLines() ){
        str_bb.push_back(line);
    }

    for( int j = 0; j < str_bb.size(); j++ ){
        auto string = str_bb[j];
        auto separator = std::string(" ");
        auto separator_length = separator.length();
        auto list = std::vector<std::string>();

        if (separator_length == 0) {
            list.push_back(string);
        }
        else{
            auto offset = std::string::size_type(0);
            while (1) {
                auto pos = string.find(separator, offset);
                if (pos == std::string::npos) {
                    list.push_back(string.substr(offset));
                    break;
                }
                list.push_back(string.substr(offset, pos - offset));
                offset = pos + separator_length;
            }
        }


        if( list.size() == 5 ){
            float x,y,w,h;
            x = ofToFloat(list[1]);
            y = ofToFloat(list[2]);
            w = ofToFloat(list[3]);
            h = ofToFloat(list[4]);
            x = x -w/2.0;
            y = y -h/2.0;
            ofRectangle r(x,y,w,h);
            TrainObject t(ofToInt(list[0]), classNamesVec.at(ofToInt(list[0])), r);
            train.push_back(t);
        }
    }
}

void ofxYolov2::saveAnnotation()
{
    saveBoundingBoxToFile(filename_txt);
}
void ofxYolov2::loadAnnotationDir(string _path_to_file)
{
    train.clear();
    filepath = _path_to_file;
    pos_annotation_file = 0;
    dir_annotation.open(filepath);
    dir_annotation.allowExt("jpg");
    dir_annotation = dir_annotation.getSorted();

    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");

    // Load first jpg image
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
}
void ofxYolov2::loadAnnotationImage(string _path_to_file)
{
    image_annotation.load(_path_to_file);
    update(image_annotation.getPixels());
}

void ofxYolov2::saveBoundingBoxToFile(string _path_to_file)
{
    ofFile file;
    if( file.open(_path_to_file, ofFile::WriteOnly) ){
        for( int i = 0; i < train.size(); i++){
            train[i].r.standardize();
            file <<
            train[i].id << " " <<
            train[i].r.x + train[i].r.getWidth()/2.0 << " " <<
            train[i].r.y + train[i].r.getHeight()/2.0 << " " <<
            train[i].r.getWidth() << " " <<
            train[i].r.getHeight() << endl;
        }
        file.close();
        train.clear();
        loadBoundingBoxFile(_path_to_file);
    }
    else{
        cout << "Failed: _path_to_file: " + _path_to_file << endl;
    }

}

void ofxYolov2::setNextAnnotation()
{
    pos_annotation_file++;
    if( pos_annotation_file > dir_annotation.size()-1 ){
        pos_annotation_file = dir_annotation.size()-1;
    }
    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
}

void ofxYolov2::setPreviousAnnotation()
{
    pos_annotation_file--;
    if( pos_annotation_file < 0 ){
        pos_annotation_file=0;
    }
    filename_jpg = dir_annotation.getPath(pos_annotation_file);
    filename_txt = Replace(dir_annotation.getPath(pos_annotation_file),".jpg", ".txt");
    loadAnnotationImage(filename_jpg);
    loadBoundingBoxFile(filename_txt);
}

void ofxYolov2::drawClassSelector(float _x, float _y, int _row)
{
    // Show Class Selector
    int row = _row;
    float x,y;

    float w_max = 0;
    for( int i = 0; i < classNamesVec.size(); i++ ){
        float w_tmp = font_info.getStringBoundingBox(classNamesVec.at(i), 0, 0).getWidth();
        if( w_tmp > w_max ){
            w_max = w_tmp;
        }
    }
    ofRectangle r_area(_x,
                       _y,
                       w_max*row,
                       14*classNamesVec.size()/row+14);

    ofNoFill();
    ofDrawRectangle(r_area);
    font_info.drawString("Select a class name",r_area.x,r_area.y);
    for( int i = 0; i < classNamesVec.size(); i++ ){
        x = _x+w_max*(i%row);
        y = _y+14+14*(i/row);
        ofNoFill();
        ofSetColor(detection_color.at(i));
        ofRectangle r_name;
        r_name.set(font_info.getStringBoundingBox(classNamesVec.at(i),
                                                  x,y));

        if( r_name.inside(ofGetMouseX(),ofGetMouseY()) ){
            if( ofGetMousePressed() ){
                class_id_selected = i;
            }
            else{
                ofDrawRectangle(r_name);
            }
        }

        if( class_id_selected == i ){
            ofFill();
            ofDrawRectangle(r_name);

            ofSetColor(detection_color.at(i).getInverted());
            font_info.drawString(classNamesVec.at(i),x,y);

        }
        else{
            font_info.drawString(classNamesVec.at(i),x,y);
        }
    }

}

void ofxYolov2::addTrainObject(ofRectangle _r)
{
    train.push_back(TrainObject(class_id_selected, classNamesVec.at(class_id_selected), _r));
}

