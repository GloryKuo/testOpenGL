#include <gl\glut.h>

float yRot = 0;
unsigned int nPre = 0;
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // 清楚颜色数据和深度数据（清屏）
    glColor3f(1.0,1.0,1.0);
    glLoadIdentity();        //初始变换矩阵为单位矩阵

    GLfloat no_mat[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[] =  {1.0, 1.0, 1.0, 1.0};
    GLfloat no_shininess[] = {0.0};
    GLfloat low_shininess[] = {5.0};
    GLfloat hig_shininess[] = {100.0};
    GLfloat mat_emission[] = {0.3,0.8, 0.0, 1.0};



    glPushMatrix();            //压入变换矩阵那

    //材质设置有效一直保持到改变为止。

    //第一行第一列的球，仅有慢反射光，而无环境光喝镜面光。
    glPushMatrix();
    glTranslatef(-3.75, 3.0, 0.0);    //第一个球的位置
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    glutSolidSphere(1, 30,30);//利用库函数绘制一个半径为1的球体。
    glPopMatrix();            //弹出矩阵。

    //第一行第二列的球，有漫反射光喝镜面光，低高光，并无环境光
    glPushMatrix();
    glTranslatef(-1.25, 3.0, 0.0);
    glMaterialfv(GL_FRONT,GL_AMBIENT, no_mat);    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS, low_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    glutSolidSphere(1, 30,30);//利用库函数绘制一个半径为1的球体。
    glPopMatrix();

    //第一行第三列绘制的球有漫反射光和镜面光，并有很亮的高光，而无环境光 
    glPushMatrix();
    glTranslatef(1.25, 3.0, 0.0);
    glMaterialfv(GL_FRONT,GL_AMBIENT, no_mat);    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS, hig_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    glutSolidSphere(1, 30,30);//利用库函数绘制一个半径为1的球体。
    glPopMatrix();

    //第一行第四列绘制的球有漫反射光和辐射光，而无环境和镜面反射光
    glPushMatrix();
    glTranslatef(3.75, 3.0, 0.0);
    glMaterialfv(GL_FRONT,GL_AMBIENT, no_mat);    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT,GL_SHININESS, hig_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glutSolidSphere(1, 30,30);//利用库函数绘制一个半径为1的球体。
    glPopMatrix();

    glPopMatrix();        //这里会沿用以前的设置的材质。
    glTranslatef(3.75, 0.0, 0.0);
    glutSolidSphere(1, 30,30);//利用库函数绘制一个半径为1的球体。
    glPopMatrix();

    
    //其他的不写了。。
    //第二行第一列绘制的球有漫反射光和环境光，而镜面反射光。
    //第二行第二列绘制的球有漫反射光、环境光和镜面光，且有低高光
    //第二行第三列绘制的球有漫反射光、环境光和镜面光，且有很亮的高光
    //第二行第四列绘制的球有漫反射光、环境光和辐射光，而无镜面光
    //第三行第一列绘制的球有漫反射光和有颜色的环境光，而无镜面光。
    //第三行第二列绘制的球有漫反射光和有颜色的环境光以及镜面光，且有低高光
    //第三行第三列绘制的球有漫反射光和有颜色的环境光以及镜面光，且有很亮的高光
    //第三行第四列绘制的球有漫反射光和有颜色的环境光以及辐射光，而无镜面光
    glutSwapBuffers();            //交换缓冲区。显示图形
}

//初始化
void init (void) 
{
    glClearColor (0.5, 0.5, 0.5, 0.0);    //清理颜色，为黑色，（也可认为是背景颜色）

    
    GLfloat light_diffuse[]= { 0.0, 0.0, 1.0, 1.0};//蓝色        //有灯光才能体现材质的效果，他的灯光的强度是与的关系。貌似是两个相乘什么的。(0-1)范围。
    GLfloat light_position[] = { 0, 3, 2.0, 0.0 };        //设置点光源的矩阵，这个向量也忒奇怪了，1不跟着变，0跟着变，设置为透视之后又是1跟着变，0不跟着变。
    GLfloat light_specular[] = { 1.0, 1.0, 0.0, 1.0 };        //反射光
    GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);        //，漫射光会产生漫射的效果和高光的效果
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);    //点光源没有漫射光的效果，会一直是黑色的。但会有高光反射的效果。
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);        //反射光基本没有效果。可能是我不知道吧

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    //如果什么都不设置，GL_LIGHT有默认的值。
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);    
    glEnable(GL_DEPTH_TEST);
}

//当窗口大小改变时，会调用这个函数
void reshape(GLsizei w,GLsizei h)
{
    //这里小说明一下：矩阵模式是不同的，他们各自有一个矩阵。投影相关
    //只能用投影矩阵。(只是目前情况下哦，等我学多了可能就知道为什么了。)

    glViewport(0,0,w,h);        //设置视口
    glMatrixMode(GL_PROJECTION);    //设置矩阵模式为投影变换矩阵，
    glLoadIdentity();                //变为单位矩阵
    //gluPerspective(60, (GLfloat)w / h, 0, 1000);    //设置投影矩阵
    glOrtho(-6.0, 6.0, -6.0 * h / w, 6.0* h / w, -10, 10);    //为了不变形，则要长和宽成比例
    glMatrixMode(GL_MODELVIEW);        //设置矩阵模式为视图矩阵(模型)
    glLoadIdentity();                //变为单位矩阵
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);        //Opnegl初始化
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA); //设置显示模式为双缓冲，RGEBA

    glutInitWindowSize (800, 600);            //窗口大小
    glutInitWindowPosition (100, 100);        //窗口位置
    glutCreateWindow ("hello");                //创建一个标题为hello的窗口
    init ();                                //初始化资源,这里一定要在创建窗口以后，不然会无效。
    glutDisplayFunc(display);                //窗口大小改变时的回调
    glutReshapeFunc(reshape);                //绘制图形时的回调
    glutMainLoop();                            //主循环。
    return 0;  
}