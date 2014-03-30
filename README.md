Color Transfert
=====================


Apply the color of an image to another one, keeping natural color and avoiding artificial effects that can come from color manipulation.

Given 2 images,
- `input image`: you want to change the colors of this one
- `color image`: you want to apply the colors of this image to the first one.
Transfert the colors of the `color image` to the `input image`.

----------


Examples
---------

**ColorTransfert** is really easy to use but

> **Warning:**
>
> - ColorTransfert works the best with natural image. The algorithm used is very sensitive to natural color. Artificial colors like painting may return a worse result.

> - Some images may not be compatible and return poor results. If the two images presents very different color composition the algorithm could work poorly. An image with only one color and the other with a lot of diff


#### <i class="icon-file"></i> Source
This is an implementation of the algorithm describes http://www.thegooch.org/Publications/PDFs/ColorTransfer.pdf

Installation
---------

#### Dependencies
**ColorTransfert** depends on  `openCV` and `boost::program_options`. Make sure you have them on you system before trying to compile.

- install opencv
- dowload boost::program_options header

#### Compilation
The compilation process use [CMake](http://www.cmake.org/). Checkout the code, create a specific folder for your build and use cmake for the compilation. If you don't have cmake, take a look at the [documentation](http:.//opencv.org/quickstart.html).
> git clone http://... color_transfert
> cd color_transfer
> mkdir build
> cd build
> cmake ../
> make

Usage
---------

> ./transfert -i path/input/image -c path/color/image -o path/output/image
> ./transfert -h



#### <i class="icon-file"></i> Credits
> [StackEdit](https://stackedit.io/): mardown editor
> [CMake](http://www.cmake.org/): cross-platform open source build system.
> [OpenCV](http://www.opencv.org/): open source computer vision platform.
> [Boost](http://www.boost.org/): c++ libraries under the Boost Software License
