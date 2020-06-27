#pragma once

#include <fstream>

#include "../Dots/SRawData.h"
#include "../Dots/CWorld.h"
#include "../Dots/CShowWnd.h"
#include "CWorldParams.h"


class CTextArchiver
{
public:
    bool Save(LPCTSTR strFileName, CShowWnd& sw)
    {
        bool bRes = false;
        if (sw.pWorld)
        {
            try
            {
                std::ofstream ofs(strFileName);
                SaveData(ofs, sw.pWorld->rdRawData, sw.bmpBodys);
                bRes = true;
            }
            catch (...)
            {
                bRes = false;
            }
        }
        return bRes;
    }


    bool Load(LPCTSTR strFileName, CShowWnd& sw, const CWorldParams& wpRef, CRenderTarget* prt)
    {
        bool bRes = false;
        if (sw.pWorld)
        {
            try
            {
                std::ifstream ifs(strFileName);
                bRes = LoadData(ifs, sw.pWorld->rdRawData, sw.bmpBodys, wpRef, prt);
            }
            catch (...)
            {
                bRes = false;
            }
        }
        return bRes;
    }

protected:
    void SaveData(std::ostream& os, SRawData& rd, DotBmpPointerVector& bmps)
    {
        os << rd.fG << std::endl;
        os << rd.nAmount << "\t" << rd.nComm << "\t" << rd.nGiant << "\t" << rd.nBlackhole << "\t" << 
            rd.nAntiComm << "\t" << rd.nAntiGiant << "\t" << rd.nAntiBlackhole << std::endl;
        for (int i = 0; i < rd.nAmount; i++)
        {
            os << int(rd.eaStarType[i]) << "\t";
            os << bmps[i]->Size() << "\t";
            os << rd.faGm[i] << "\t";
            os << rd.naLooseLevel[i] << "\t";
            os << rd.faPosX[i] << "\t";
            os << rd.faPosY[i] << "\t";
            os << rd.faVelocityX[i] << "\t";
            os << rd.faVelocityY[i] << "\t";
            //os << rd.faAntiG[i] << "\t";
            os << rd.caColor[i] << std::endl;
        }
    }

    bool LoadData(std::istream& is, SRawData& rd, DotBmpPointerVector& bmps, const CWorldParams& wpRef, CRenderTarget* prt)
    {
        bool bRes = false;
        float fG;
        int nComm{ 0 }, nGiant{ 0 }, nBlackhole{ 0 }, nAntiComm{ 0 }, nAntiGiant{ 0 }, nAntiBlackhole{ 0 };
        int nAmount{ 0 };
        is >> fG >> nAmount >> nComm >> nGiant >> nBlackhole >> nAntiComm >> nAntiGiant >> nAntiBlackhole;
        if (wpRef.IsMatch(int(fG), nAmount, nComm, nGiant, nBlackhole, nAntiComm, nAntiGiant, nAntiBlackhole))
        {
            int n;
            COLORREF clr;
            rd.Prepare(nComm, nGiant, nBlackhole, nAntiComm, nAntiGiant, nAntiBlackhole, wpRef.GetThreadCount());
            rd.fG = fG;
            for (int i = 0; i < nAmount; i++)
            {
                is >> n;
                rd.eaStarType[i] = EStarType(n);
                is >> n;    // diameter
                rd.faRadius[i] = float(n) / 2.f;
                is >> rd.faGm[i];
                is >> rd.naLooseLevel[i];
                is >> rd.faPosX[i];
                is >> rd.faPosY[i];
                is >> rd.faVelocityX[i];
                is >> rd.faVelocityY[i];
                is >> clr;
                rd.caColor[i] = clr;
                // now padding other
                rd.Padding(i);
                // now create bmp
                EDotType dt = EDotType::dtNormal;
                if (rd.eaStarType[i] == EStarType::stBlackhole || rd.eaStarType[i] == EStarType::stAnti_Blackhole)
                    dt = EDotType::dtBlackHole;
                bmps.push_back(std::make_shared<CDotBmp>(n, clr, prt, rd.naLooseLevel[i] == 0, dt));
            }
            bRes = true;
        }
        return bRes;
    }
};

