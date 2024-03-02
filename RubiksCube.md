### Cube indexing
Indexing will go from top to bottom, left to right, and back to front.

top-left-back: 0 
top-middle-back: 1
top-right-back: 2 <br/>
top-left-middle: 3
top-middle-middle: [4]
top-right-middle: 5 <br/>
top-left-front: 6
top-middle-front: 7
top-right-front: 8


middle-left-back: 9
middle-middle-back: [10]
middle-right-back: 11 <br/>
middle-left-middle: [12]
middle-middle-middle: [13]
middle-right-middle: [14] <br/>
middle-left-front: 15
middle-middle-front: [16]
middle-right-front: 17 <br/>

bottom-left-back: 18
bottom-middle-back: 19
bottom-right-back: 20 <br/>
bottom-left-middle: 21
bottom-middle-middle: [22]
bottom-right-middle: 23 <br/>
bottom-left-front: 24
bottom-middle-front: 25
bottom-right-front: 26

### Operations

#### clockwise top 0
0 1 2   
3 4 5  
6 7 8

-->
6 3 0
7 4 1
8 5 2

<--
2 5 8
1 4 7
0 3 6


rotation axis-y
* 0 --> 2
* 1 --> 5
* 2 --> 8
* 3 --> 1
* 4 --> 4
* 5 --> 7
* 6 --> 0
* 7 --> 3
* 8 --> 6

#### clockwise bottom 2
* 18 --> 20
* 19 --> 23
* 20 --> 26
* 21 --> 19
* 22 --> 22
* 23 --> 25
* 24 --> 18
* 25 --> 21
* 26 --> 24

#### clockwise left 0
rotation axis-x
0  3  6
9  12 15
18 21 24
 -->
18 9  0
21 12 3
24 15 6
<--
6 15 24
3 12 21
0 9  18

* 0 --> 6
* 3 --> 15
* 6 --> 24
* 9 --> 
* 12
* 15
* 18
* 21
* 24



class rubiksCube

 clockwiseRotationMapping:
 {
 0 --> 2,
 1 --> 5,
 2 --> 8,
 3 --> 1,
 4 --> 4,
 5 --> 7,
 6 --> 0,
 7 --> 3,
 8 --> 6 
 }

for i in cubeSlots.length
auto NewPosition = indexToPos[i]
 cube[i].moveTo(NewPosition)

class RubiksCubeModel:

array 26 indexes
 0 1 2 3 4 5 6 7 8
[cube(id213) , , , , , , , , ,]


void topOperation(bool counterClock){
    faceInxs = [0,1,2,3,4,5,6,7,8]
}

bottomOperation(bool counterClock){
 faceInxs = [18,19,20,21,22,23,24,25,26]

 makeFaceRotation(faceInxs, ture)
 
 
 
 
}


}

makeFaceRotation(faceInxs, counter){
 int outPutArray = [9]

 for i = 0 until 9 {
  outPutArray[clockwiseRotationMapping[i]] = faceInxs[i]
 }

return outPutArray
 

}


leftOperation(bool counterClock)

rightOperation(bool counterClock)

backOperation()

frontOperation()