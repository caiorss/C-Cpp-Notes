/** Author: Caio Rodrigues
 *  Brief:  Shared Pointer Experiment demonstrating its functionalities.
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

#include <cassert> // assert (assertions)
#include <memory> // Provide: std::shared_ptr

// Media asset interface => generic specification for an media asset.
// Note: A struct is just a class with everything public by default.
struct IMediaAsset{
    virtual std::string type() const = 0;
    virtual std::string name() const = 0;
    virtual void play()              = 0;

    /** Always add a default destructor. */
    virtual ~IMediaAsset() = default;
};


class MusicAsset: public IMediaAsset
{
    std::string m_name;
public:

    MusicAsset(std::string name):
        m_name(std::move(name))
    {
        std::printf(" [TRACE] CTOR - Music object => name = %s created. \n "
                    , m_name.c_str());
    }

    MusicAsset(): MusicAsset("unnamed-music") { }

    ~MusicAsset()
    {
        std::printf(" [TRACE] DTOR - Music object => name = %s destroyed. \n "
                    , m_name.c_str());
    }

    // Override keyword is optional, but it is recommended in C++11
    std::string type() const override
    {
        return "music";
    }

    std::string name() const override
    {
        return m_name;
    }

    void play() override
    {
        std::cout << " [INFO] Playing music: " << m_name << std::endl;
    }

    void setVolumePercent(unsigned int volume) {
        std::cout << " [INFO] Volume of music <"
                  << m_name << "> set to "
                  << volume << "%\n";
    }

};

class PictureAsset: public IMediaAsset
{
    std::string m_name;
public:

    PictureAsset(std::string name):
        m_name(std::move(name))
    {
        std::printf(" [TRACE] CTOR - Picture object => name = %s created. \n "
                   , m_name.c_str());
    }

    PictureAsset(): PictureAsset("unnamed-picture") { }

    ~PictureAsset()
    {
        std::printf(" [TRACE] DTOR - Picture object => name = %s destroyed. \n "
                    , m_name.c_str());
    }

    // Override keyword is optional, but it is recommended in C++11
    std::string type() const override
    {
        return "picture";
    }

    std::string name() const override
    {
        return m_name;
    }

    void play() override
    {
        std::cout << " [INFO] Show picture : " << m_name << std::endl;
    }

    void resizeInPercent(unsigned int percent) {
        std::cout << " [INFO] Picture<"
                  << m_name << "> resized "
                  << percent << "%\n";
    }


};

// Type alias for shared_ptr
template<typename T>
using sh = std::shared_ptr<T>;

/* For the factory function use std::unique_ptr instead of shared_ptr as
 * it is easier to convert unique_ptr to shared_ptr than the other way around.
 */
std::unique_ptr<IMediaAsset>
factoryFunction(int id)
{
    if(id == 1) { return std::make_unique<MusicAsset>("Fur Elise"); }
    if(id == 2) { return std::make_unique<PictureAsset>("São Paulo's Skyline"); }
    if(id == 3) { return std::make_unique<MusicAsset>("Baorque music"); }
    if(id == 4) { return std::make_unique<PictureAsset>("Tokyo skyline"); }
    return nullptr;
}


int main()
{

    std::cout << std::boolalpha;

    //=============== EXPERIMENT 1 ===============================================//
    std::puts("\n ****** =>>>> EXPERIMENT 1 - Reference counter test **************\n");

    // Not recommended using new => Instead use std::make_shared
    // When the object is created, the reference count is set to 1.
    std::shared_ptr<PictureAsset> pic1(new PictureAsset("Desert sunshine"));
    assert( pic1.use_count() == 1);

    std::cout << " => Type of object pic1 = " << pic1->type()
              << "  ; name = " << pic1->name()
              << std::endl;

    std::cout << " => Address of pic1 managed object = " << pic1.get() << std::endl;
    std::cout << " => Value of ref. counter of pic1 = " << pic1.use_count() << std::endl;


    {
        std::puts(" >> [TRACE] Before entering local scope <<=== \n");

        // Increments reference counter by 1 (set to 2) => .use_count() returns 2
        std::shared_ptr<PictureAsset> pic1_copyA = pic1;
        assert(pic1_copyA.use_count() == 2);
        assert(pic1.use_count() == 2);
        std::printf(" [INFO] Reference count of pic1_copyA = %ld \n", pic1_copyA.use_count());
        std::printf(" pic1_copyA->name() = %s \n", pic1_copyA->name().c_str());


        // Increments reference counter by 1 (set to 3)
        auto pic1_copyB = pic1;
        assert(pic1.use_count() == 3);
        assert(pic1_copyB.use_count() == 3);
        // .use_count() returns 3
        std::printf(" [INFO] Reference count of pic1_copyB = %ld \n", pic1_copyA.use_count());
        std::printf(" pic1_copyB->name() = %s \n", pic1_copyB->name().c_str());

        /** Objdcts pic1_copyA and pic1_copyB are destroyed,
         * then each one decrements the reference counter by 1
         */
        std::puts(" >> [TRACE] Levaing  local scope <<=== ");
    }

    // .use_count returns 1 as the two copies were destroyed.
    std::printf(" [INFO] Before reset(). Reference count of pic1 = %ld \n", pic1.use_count());

    assert(pic1.use_count() == 1);
    // Decrements reference counter of control block by 1.
    // The owned object and the control block are deleted when the counter is zero.
    // When the counter reaches zero, the internal pointer is set to nullptr.
    pic1.reset();
    assert(pic1.use_count() == 0);

    std::printf(" [INFO] After reset(). Reference count of pic1 = %ld \n", pic1.use_count());

    std::cout << " ?? Is pic1 nullptr => (pic1 == nullptr) : "
              << (pic1  == nullptr)
              << std::endl;

    std::cout << " ?? Does shared_ptr pic1 own an object => (pic1 != nullptr) : "
              << (pic1  != nullptr)
              << std::endl;

    //======= EXPERIMENT 2 ===>> Polymorphism ======================================//
    std::puts("\n ****** =>>>> EXPERIMENT 2 - Polymorphism test **************\n");

    std::printf(" \n --- Experiment 2.A - musicA object \n");

    // Default initialized to nullptr (reference counter is zero).
    std::shared_ptr<IMediaAsset> media1;
    assert(media1.use_count() == 0);

    if(!media1){
        std::cout << " [TRACE] Media pointer owns no object, it is nullptr" << std::endl;
    } else {
        std::cout << " [TRACE] Media pointer owns an object, it is not nullptr" << std::endl;
    }
    std::printf(" [INFO] Ref count of media1 = %ld \n", media1.use_count());

    // Best way to instantiante a polymorphic object
    // Type: std::shared_ptr<MusicAsset>
    auto musicA = std::make_shared<MusicAsset>("Fur Elise");
    std::printf(" [INFO] Ref count of musicA = %ld \n", musicA.use_count());

    // Polymorphism: the pointer to a derived class
    // can be assigned to a pointer of the base class.
    media1 = musicA;
    std::printf(" [INFO] Ref count of musicA = %ld \n", musicA.use_count());
    std::printf(" [INFO] Ref count of medai1 = %ld \n", media1.use_count());

    std::printf(" \n --- Experiment 2.B - pictA object \n");
    {


        // Type: std::shared_ptr<PictureAsset>
        auto pictA = std::make_shared<PictureAsset>("Canyon sunset");
        std::printf(" [INFO] Before assigning: Ref count of pictA = %ld \n"
                    , pictA.use_count());

        std::printf(" => pictA->type() = '%s' - pictA->name() = %s  "
                    , pictA->type().c_str(), pictA->name().c_str() );

        media1 = pictA;
        std::printf(" [INFO] After assigning: Ref count of pictA = %ld \n"
                    , musicA.use_count());

        std::printf(" => media1->type() = '%s' - media1->name() = %s  \n"
                    , media1->type().c_str(), media1->name().c_str() );

        // Object (shared pointer) pictA destroyed here
        // , but not the owned object "Canyon sunset" as it also owned by 'media1' pointer
    }

    std::printf(" \n --- Experiment 2.C - After end of local scope \n");

    std::shared_ptr<IMediaAsset> media2 = media1;
    std::printf(" [INFO] Ref count of media1 = %ld \n", media1.use_count());
    std::printf(" [INFO] Ref count of media1 = %ld \n", media2.use_count());

    media1 = nullptr;
    std::printf(" [INFO] Ater setting media1 to null => Ref count of media1 = %ld \n"
                , media1.use_count());
    std::printf(" [INFO] Ater setting media2 to null => Ref count of media2 = %ld \n"
                , media2.use_count());

    std::printf(" Before setting media2 to nullptr => media2->type() = '%s' - media2->name() = %s  \n"
                , media2->type().c_str(), media2->name().c_str() );

    // Reference count set to zero => The object "Canyon sunset" is destroyed
    // as any pointer no longer owns it.
    media2 = nullptr;

    std::cout << " After setting media2 to nullptr => ?? Is media2 nullptr = "
              << (media2 == nullptr) << std::endl;

    //=============== EXPERIMENT 3 - Polymorphism and pointer casting ===========//
    std::puts("\n ****** =>>>> EXPERIMENT 3 - Polymorphism and casting ***********\n");

    std::shared_ptr<IMediaAsset> asset1 = std::make_shared<MusicAsset>("Blues");


    // Downcast object at runtime (RTTI) checking whetehr it
    // is a Music asset taking its ownership.
    {
        // RTTI Runtime - Type information
        std::shared_ptr<PictureAsset> asset_is_music = std::dynamic_pointer_cast<PictureAsset>(asset1);
        if(asset_is_music){
            std::cout << " =>>> Asset type is music OK" << std::endl;
            std::printf(" Object Type = %s - object name = %s\n"
                        , asset_is_music->type().c_str()
                            , asset_is_music->name().c_str());

        } else {
            std::cout << " =>> Asset type is not music. " << std::endl;
        }

    }

    // Static casting object at runtime => IF the casting is wrong, it results in
    // undefined behavior!!

    std::cout << "\n ---- Before changing pointed object " << std::endl;
    asset1 = factoryFunction(2);
    std::cout << "\n ---- After changing pointed object " << std::endl;

    if(asset1->type() == "picture")
    {
        // Warning: if the casting does not match, results in undefined behavior!!
        auto asset_picture = std::static_pointer_cast<PictureAsset>(asset1);

        std::cout << "\n =>>> Asset type is picture OK" << std::endl;
        std::printf(" Object Type = %s - object name = %s\n"
                    , asset_picture->type().c_str()
                        , asset_picture->name().c_str());

        asset_picture->resizeInPercent(40);
    }


    //=============== EXPERIMENT 4 - Polymorphism and containers ================//
    std::puts("\n ****** =>>>> EXPERIMENT 4 - Polymorphioc Objects and Containers *****\n");

    auto objA = std::make_shared<MusicAsset>("Some medieval music");
    std::shared_ptr<IMediaAsset> objB = factoryFunction(2);

    std::cout << "\n --- Before filling 'collection' " << std::endl;

    // Polymorphism
    std::vector<sh<IMediaAsset>> collection =
        {
            std::make_shared<PictureAsset>("Max turbo power diesel engine")
          , std::make_shared<MusicAsset>("Some baroque music")
          , std::make_shared<PictureAsset>("Desert canyon")
          , factoryFunction(1)
        };

    collection.push_back(objA);
    collection.push_back(objB);

    std::cout << " --- After filling 'collection' \n\n";

    std::cout << " Ref counter of objA = " << objA.use_count() << std::endl;

    std::cout << " ==== Print objects in the container ==== " << std::endl;

    for(auto const& ptr: collection){
        std::cout << std::right << std::setw(15) << ptr->type()
                  << std::right << "  "
                  << std::left  << std::setw(35) << ptr->name()
                  << std::endl;
    }
    std::cout << std::right;

    std::cout << "\n ----- Before clearing the vector  ----- " << std::endl;

    collection.clear();

    std::cout << "\n ----- After clearing the vector  ----- " << std::endl;

    objA = nullptr;
    objB.reset();

    std::puts("\n  *********** End of Main() *****************");

    return 0;
}
