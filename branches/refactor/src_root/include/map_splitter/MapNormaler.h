#ifndef MapNormaler_H
#define MapNormaler_H

#include "Ogre.h"

namespace Ogre
{
    class MapNormaler
    {
        public:
            MapNormaler();

            ~MapNormaler();
        
            Image *getNormalMap ();

            Image getNormalAt ();

            void reset();

            const Vector3 getNormalAt (const uint x, const uint z);

            Vector3 *getNormalData();

        private:

            void CalcNormalMap();


            bool mAlreadyComputed;
            uint mHeight;
            uint mWidth;
            Image NormalMap;
            
            Vector3 *Normals;
    };
}
#endif //MapNormaler_H
