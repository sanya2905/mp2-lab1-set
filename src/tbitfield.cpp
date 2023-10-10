// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

const int BitsInByte = 8;
const int BitsInElem = sizeof(TELEM) * BitsInByte;
const TELEM MaxElem = ~TELEM(0);

TBitField::TBitField(int len)
{
    if (len < 0)
    {
        throw invalid_argument("bitfield length can't be negative");
    }

    BitLen = len;
    MemLen = (BitLen / BitsInElem) + ((BitLen % BitsInElem) != 0);
    pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    copy(bf.pMem, bf.pMem + MemLen, pMem);
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / BitsInElem;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % BitsInElem);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
    {
        throw out_of_range("non-existent bit");
    }

    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
    {
        throw out_of_range("non-existent bit");
    }

    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
    {
        throw out_of_range("non-existent bit");
    }

    return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (*this == bf)
    {
        return *this;
    }

    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen)
    {
        MemLen = bf.MemLen;
        TELEM* newMem = new TELEM[MemLen];
        delete[] pMem;
        pMem = newMem;
    }
    copy(bf.pMem, bf.pMem + MemLen, pMem);
    
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != BitLen)
    {
        return 0;
    }

    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
        {
            return 0;
        }
    }

    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    const TBitField* right = (BitLen > bf.BitLen) ? &bf : this;

    TBitField res((BitLen > bf.BitLen) ? *this : bf);
    for (int i = 0; i < min(MemLen, bf.MemLen); i++)
    {
        res.pMem[i] |= right->pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField res(max(BitLen, bf.BitLen));
    for (int i = 0; i < min(MemLen, bf.MemLen); i++)
    {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);

    for (int i = 0; i < MemLen; i++)
    {
        res.pMem[i] = ~pMem[i];
    }
    res.pMem[MemLen - 1] &= (MaxElem >> (BitsInElem - BitLen % BitsInElem));

    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char c;
    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> c;
        if (c == '1')
        {
            bf.SetBit(i);
        }
        else
        {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i))
        {
            ostr << '1';
        }
        else
        {
            ostr << '0';
        }
    }
    return ostr;
}
