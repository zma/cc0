void main()
{
    asm ("mov   $0x3:msq, 0x200001010:msq");
    asm ("exit:c");
}